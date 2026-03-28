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
 * Patching happens in ApplyFsPatches(), which is called from
 * kern_initial_process.cpp right after KInitialProcessReader::Load() decompresses
 * the FS binary into a KPageGroup, but before KProcess::Initialize().
 *
 * Why this approach?
 *   - We have full kernel access to the still-uninitialized decompressed image.
 *   - No user-mode sysmodule or debug services required.
 *   - Extremely early patching (before FS even starts running).
 *
 * Patch Values:
 *   - AArch64Nop  (0xD503201F) → stored in memory as 1F 20 03 D5
 *   - AArch64Mov0 (0x2A1F03E0) → stored in memory as E0 03 1F 2A
 */

#pragma once
#include <mesosphere.hpp>

#ifdef ATMOSPHERE_BOARD_NINTENDO_NX
    ams::TargetFirmware GetTargetFirmware();
#else
    consteval ALWAYS_INLINE ams::TargetFirmware GetTargetFirmware() {
        return ams::TargetFirmware_Current;
    }
#endif

namespace ams::kern {

    /* FS program ID. Only this process gets patched. */
    constexpr u64 FsProgramId = 0x0100000000000000ul;

    /* AArch64 instructions used for patching (little-endian 32-bit values) */
    constexpr u32 AArch64Nop  = 0xD503201Fu;   // nop
    constexpr u32 AArch64Mov0 = 0x2A1F03E0u;   // mov w0, #0 or mov w0, wzr

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

    NOINLINE void ApplyFsPatches(u64            program_id,
                                 KVirtualAddress /*kip_start*/,
                                 size_t          /*binary_size*/,
                                 KPageGroup     *process_pg)
    {
        /* Fast filter - only patch FS */
        if (program_id != FsProgramId) {
            return;
        }

        /* Firmware-specific offset patching (requires >= 17.0.0) */
        const auto current_firmware = GetTargetFirmware();

        if (current_firmware < ATMOSPHERE_TARGET_FIRMWARE_17_0_0) {
            return;
        }

        size_t noncasigchk_offset = 0;
        size_t nocntchk_offset    = 0;

        if (current_firmware == ATMOSPHERE_TARGET_FIRMWARE_17_0_0 ||
            current_firmware == ATMOSPHERE_TARGET_FIRMWARE_17_0_1) {
            noncasigchk_offset = 0x24154;
            nocntchk_offset    = 0x73B04;
        } else if (current_firmware == ATMOSPHERE_TARGET_FIRMWARE_18_0_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_18_0_1 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_18_1_0) {
            noncasigchk_offset = 0x245F4;
            nocntchk_offset    = 0x743BC;
        } else if (current_firmware == ATMOSPHERE_TARGET_FIRMWARE_19_0_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_19_0_1) {
            noncasigchk_offset = 0x21478;
            nocntchk_offset    = 0x745A0;
        } else if (current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_0_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_0_1 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_1_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_1_1 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_1_5 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_2_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_3_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_4_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_20_5_0) {
            noncasigchk_offset = 0x23B88;
            nocntchk_offset    = 0x7A780;
        } else if (current_firmware == ATMOSPHERE_TARGET_FIRMWARE_21_0_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_21_0_1 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_21_1_0 ||
                   current_firmware == ATMOSPHERE_TARGET_FIRMWARE_21_2_0) {
            noncasigchk_offset = 0x26960;
            nocntchk_offset    = 0x7FA88;
        } else if (current_firmware == ATMOSPHERE_TARGET_FIRMWARE_22_0_0) {
            noncasigchk_offset = 0x26838;
            nocntchk_offset    = 0x8019C;
        } else {
            /* Unsupported / future firmware – do not patch (safe default) */
            return;
        }

        /* Apply the two version-specific patches via the existing helper */
        FsKipPatch patches[2] = {
            { noncasigchk_offset, AArch64Nop  },   /* noncasigchk_patch */
            { nocntchk_offset,    AArch64Mov0 }    /* nocntchk_patch    */
        };

        for (size_t i = 0; i < 2; ++i) {
            ApplySingleFsPatch(patches[i], process_pg);
        }

    }

} /* namespace ams::kern */