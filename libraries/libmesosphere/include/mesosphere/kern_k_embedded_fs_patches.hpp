/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * FS Kernel Inline Patching (Early Boot)
 * ======================================
 *
 * This module implements firmware-aware patching of the FS (Filesystem)
 * Kernel initial process (KIP) during kernel boot.
 *
 * Patching happens in FindAndApplyFsKipPatches(), which is called from
 * kern_initial_process.cpp right after KInitialProcessReader::Load() decompresses
 * the FS binary into a KPageGroup, but before KProcess::Initialize().
 *
 * Why this approach?
 *   - We have full kernel access to the still-uninitialized decompressed image.
 *   - No user-mode sysmodule or debug services required.
 *   - Extremely early patching (before FS even starts running).
 *
 * Pattern Format:
 *   - Hex strings with ".." as wildcards (e.g. "0694....00..42..0091")
 *   - These represent raw bytes in memory (little-endian layout).
 *
 * Patch Values:
 *   - AArch64Nop  (0xD503201F) → stored in memory as 1F 20 03 D5
 *   - AArch64Mov0 (0x2A1F03E0) → stored in memory as E0 03 1F 2A
 *
 * Supported Patches (nocntchk + noncasigchk + legacy):
 *   - tbz-based content checks   → replaced with NOP
 *   - bl-based signature checks  → replaced with mov w0, #0 or mov w0, wzr
 */

#pragma once
#include <mesosphere.hpp>

namespace ams::kern {

    /* FS program ID. Only this process gets patched. */
    constexpr u64 FsProgramId = 0x0100000000000000ul;

    /* AArch64 instructions used for patching (little-endian 32-bit values) */
    constexpr u32 AArch64Nop  = 0xD503201Fu;   // nop
    constexpr u32 AArch64Mov0 = 0x2A1F03E0u;   // mov w0, #0 or mov w0, wzr

    /* Wildcard sentinel used during pattern matching */
    constexpr u16 REGEX_SKIP = 0x100;

    /* ===================================================================== */
    /* 1. Original offset-based patch support (kept for compatibility)       */
    /* ===================================================================== */
    struct FsKipPatch {
        size_t offset;
        u32    patch_value;
    };

    ALWAYS_INLINE void ApplySingleFsPatch(const FsKipPatch &patch, KPageGroup *pg) {
        size_t remaining = patch.offset;

        for (const auto &block : *pg) {
            const size_t block_size = block.GetNumPages() * PageSize;

            if (remaining < block_size) {
                const KVirtualAddress virt = KMemoryLayout::GetLinearVirtualAddress(block.GetAddress());
                *reinterpret_cast<u32 *>(GetInteger(virt) + remaining) = patch.patch_value;
                return;
            }

            remaining -= block_size;
        }

        MESOSPHERE_PANIC("FS KIP patch offset 0x%zx is out of range", patch.offset);
    }

    ALWAYS_INLINE size_t GetPageGroupSize(const KPageGroup *pg) {
        size_t total_size = 0;

        for (const auto &block : *pg) {
            total_size += block.GetNumPages() * PageSize;
        }

        return total_size;
    }

    ALWAYS_INLINE u8 ReadPageGroupByte(const KPageGroup *pg, size_t offset) {
        size_t remaining = offset;

        for (const auto &block : *pg) {
            const size_t block_size = block.GetNumPages() * PageSize;

            if (remaining < block_size) {
                const KVirtualAddress virt = KMemoryLayout::GetLinearVirtualAddress(block.GetAddress());
                return *(reinterpret_cast<const u8 *>(GetInteger(virt) + remaining));
            }

            remaining -= block_size;
        }

        MESOSPHERE_PANIC("FS KIP read offset 0x%zx is out of range", offset);
    }

    ALWAYS_INLINE void WritePageGroupByte(KPageGroup *pg, size_t offset, u8 value) {
        size_t remaining = offset;

        for (const auto &block : *pg) {
            const size_t block_size = block.GetNumPages() * PageSize;

            if (remaining < block_size) {
                const KVirtualAddress virt = KMemoryLayout::GetLinearVirtualAddress(block.GetAddress());
                *(reinterpret_cast<u8 *>(GetInteger(virt) + remaining)) = value;
                return;
            }

            remaining -= block_size;
        }

        MESOSPHERE_PANIC("FS KIP write offset 0x%zx is out of range", offset);
    }

    ALWAYS_INLINE u32 ReadPageGroupU32(const KPageGroup *pg, size_t offset) {
        return (static_cast<u32>(ReadPageGroupByte(pg, offset + 0)) << 0)  |
               (static_cast<u32>(ReadPageGroupByte(pg, offset + 1)) << 8)  |
               (static_cast<u32>(ReadPageGroupByte(pg, offset + 2)) << 16) |
               (static_cast<u32>(ReadPageGroupByte(pg, offset + 3)) << 24);
    }

    ALWAYS_INLINE void WritePageGroupU32(KPageGroup *pg, size_t offset, u32 value) {
        WritePageGroupByte(pg, offset + 0, static_cast<u8>((value >> 0) & 0xFF));
        WritePageGroupByte(pg, offset + 1, static_cast<u8>((value >> 8) & 0xFF));
        WritePageGroupByte(pg, offset + 2, static_cast<u8>((value >> 16) & 0xFF));
        WritePageGroupByte(pg, offset + 3, static_cast<u8>((value >> 24) & 0xFF));
    }

    /* ===================================================================== */
    /* 2. Pattern definition and constexpr parser                            */
    /* ===================================================================== */

    struct PatternData {
        u16 data[64]{};
        u8  size{};
    };

    /* Converts a pattern string (same format as main.cpp) into PatternData */
    constexpr PatternData MakePattern(const char* s) {
        PatternData p{};

        /* Skip optional 0x prefix */
        if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            s += 2;
        }

        while (*s != '\0' && p.size < util::size(p.data)) {
            if (s[0] == '.' && s[1] == '.') {
                p.data[p.size++] = REGEX_SKIP;   // wildcard
                s += 2;
            } else {
                auto hex = [](char c) -> u8 {
                    if (c >= '0' && c <= '9') return c - '0';
                    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
                    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
                    return 0;
                };
                u8 hi = hex(*s++);
                u8 lo = hex(*s++);
                p.data[p.size++] = static_cast<u16>((hi << 4) | lo);
            }
        }
        return p;
    }

    struct FsNarrowPattern {
        PatternData byte_pattern;      // parsed pattern with wildcards
        s32         inst_offset;       // offset from pattern start to the instruction
        s32         patch_offset;      // offset from instruction where we write the patch
        bool (*cond)(u32 inst);        // condition to verify it's the right instruction
        u32         patch_value;       // AArch64Nop or AArch64Mov0
        u32         min_fw_ver;        // 0 = no minimum
        u32         max_fw_ver;        // UINT32_MAX = no maximum
    };

    /* Instruction condition helpers (same as sys-patch) */
    constexpr bool bl_cond(u32 inst) {
        const auto type = inst >> 24;
        return type == 0x25 || type == 0x94 || type == 0x97;   // BL / B / BL variants
    }

    constexpr bool tbz_cond(u32 inst) {
        return ((inst >> 24) & 0x7F) == 0x36;   // TBZ / TBNZ
    }

    /* ===================================================================== */
    /* 3. All FS patterns with firmware version ranges                       */
    /* ===================================================================== */
    constexpr FsNarrowPattern g_fs_narrow_patterns[] = {
        /* Legacy patterns (moved to loader after 9.2.0) */
        { MakePattern("C8FE4739"),          -24, 0, bl_cond, AArch64Mov0,
          ATMOSPHERE_TARGET_FIRMWARE_MIN, ATMOSPHERE_TARGET_FIRMWARE_9_2_0 },

        { MakePattern("0210911F000072"),    -5,  0, bl_cond, AArch64Mov0,
          ATMOSPHERE_TARGET_FIRMWARE_MIN, ATMOSPHERE_TARGET_FIRMWARE_9_2_0 },

        /* tbz -> nop (content / counter checks) */
        { MakePattern("88..42..58"),        -4,  0, tbz_cond, AArch64Nop,
          ATMOSPHERE_TARGET_FIRMWARE_1_0_0, ATMOSPHERE_TARGET_FIRMWARE_3_0_2 },

        { MakePattern("1E4839....00......0054"), -17, 0, tbz_cond, AArch64Nop,
          ATMOSPHERE_TARGET_FIRMWARE_4_0_0, ATMOSPHERE_TARGET_FIRMWARE_16_1_0 },

        /* nocntchk - current main pattern (17.0.0+) */
        { MakePattern("0694....00..42..0091"),   -18, 0, tbz_cond, AArch64Nop,
          ATMOSPHERE_TARGET_FIRMWARE_17_0_0, ATMOSPHERE_TARGET_FIRMWARE_MAX },

        /* bl -> ret0 (signature / NACP checks) */
        { MakePattern("40F9........081C00121F05"), 2, 0, bl_cond, AArch64Mov0,
          ATMOSPHERE_TARGET_FIRMWARE_1_0_0, ATMOSPHERE_TARGET_FIRMWARE_18_1_0 },

        /* noncasigchk - current main pattern (19.0.0+) */
        { MakePattern("40F9............40B9091C"), 2, 0, bl_cond, AArch64Mov0,
          ATMOSPHERE_TARGET_FIRMWARE_19_0_0, ATMOSPHERE_TARGET_FIRMWARE_MAX },
    };

    /* ===================================================================== */
    /* 4. Pattern matching engine                                           */
    /* ===================================================================== */

    /* Try to find and apply one pattern inside the logical FS image */
    ALWAYS_INLINE bool TryApplyNarrowPattern(const FsNarrowPattern &p, KPageGroup *pg, size_t image_size) {
        for (size_t i = 0; i + p.byte_pattern.size <= image_size; ++i) {
            /* Compare pattern with wildcard support */
            size_t count = 0;
            while (count < p.byte_pattern.size) {
                const u16 pat = p.byte_pattern.data[count];
                if (pat != REGEX_SKIP && pat != ReadPageGroupByte(pg, i + count)) {
                    break;
                }
                ++count;
            }

            if (count != p.byte_pattern.size) {
                continue;
            }

            /* Resolve the instruction position in signed space first. */
            const ptrdiff_t inst_off = static_cast<ptrdiff_t>(i) + static_cast<ptrdiff_t>(p.inst_offset);
            if (inst_off < 0 || inst_off + static_cast<ptrdiff_t>(sizeof(u32)) > static_cast<ptrdiff_t>(image_size)) {
                continue;
            }

            const u32 inst = ReadPageGroupU32(pg, static_cast<size_t>(inst_off));
            if (!p.cond || !p.cond(inst)) {
                continue;
            }

            const ptrdiff_t patch_off = inst_off + static_cast<ptrdiff_t>(p.patch_offset);
            if (patch_off < 0 || patch_off + static_cast<ptrdiff_t>(sizeof(u32)) > static_cast<ptrdiff_t>(image_size)) {
                continue;
            }

            WritePageGroupU32(pg, static_cast<size_t>(patch_off), p.patch_value);
            return true;
        }

        return false;
    }

    /* Scan the entire decompressed FS image as one logical byte stream */
    void ApplyNarrowFsPatternPatches(KPageGroup *pg) {
        const u32 current_fw = GetTargetFirmware();
        const size_t image_size = GetPageGroupSize(pg);

        for (const auto &pat : g_fs_narrow_patterns) {
            if ((pat.min_fw_ver == 0 || current_fw >= pat.min_fw_ver) &&
                (pat.max_fw_ver == UINT32_MAX || current_fw <= pat.max_fw_ver)) {
                TryApplyNarrowPattern(pat, pg, image_size);
            }
        }
    }

    /* ===================================================================== */
    /* 5. Main entry point (called from kern_initial_process.cpp)            */
    /* ===================================================================== */
    NOINLINE void ApplyFsPatches(u64            program_id,
                                           KVirtualAddress /*kip_start*/,
                                           size_t          /*binary_size*/,
                                           KPageGroup     *process_pg)
    {
        /* Fast filter - only patch FS */
        if (program_id != FsProgramId) {
            return;
        }

        /* Optional: support for old-style offset-based patches */
        // for (size_t i = 0; i < patch_set->num_patches; ++i) {
        //     ApplySingleFsPatch(patch_set->patches[i], process_pg);
        // }

        /* Apply all version-appropriate pattern patches */
        ApplyNarrowFsPatternPatches(process_pg);
    }

} /* namespace ams::kern */
