#pragma once

#include <stratosphere.hpp>

namespace ams::ldr::exefs {

    constexpr u16 PatternWildcard = 0x100;

    enum class ModuleType : u8 {
        Any,
        Rtld,
        Main,
        Sdk,
        Subsdk,
        BrowserDll,
    };

    struct PatternData {
        u16 data[64]{};
        u8 size{};

        consteval PatternData(const char *s) {
            if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
                s += 2;
            }

            while (*s != '\0' && size < util::size(data)) {
                if (s[0] == '.' && s[1] == '.') {
                    data[size++] = PatternWildcard;
                    s += 2;
                } else {
                    auto high = ParseNibble(*(s++));
                    auto low = ParseNibble(*(s++));
                    data[size++] = static_cast<u16>((high << 4) | low);
                }
            }
        }

    private:
        static consteval u8 ParseNibble(char c) {
            AMS_ASSUME(('0' <= c && c <= '9') || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'));
            if ('0' <= c && c <= '9') {
                return c - '0';
            } else if ('A' <= c && c <= 'F') {
                return c - 'A' + 0xA;
            } else {
                return c - 'a' + 0xA;
            }
        }
    };

    struct PatchData {
        u8 data[20]{};
        u8 size{};

        consteval PatchData(const char *s) {
            if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
                s += 2;
            }

            while (*s != '\0' && size < util::size(data)) {
                auto high = ParseNibble(*(s++));
                auto low = ParseNibble(*(s++));
                data[size++] = static_cast<u8>((high << 4) | low);
            }
        }

        bool IsApplied(const u8 *mapped) const {
            return std::memcmp(data, mapped, size) == 0;
        }

    private:
        static consteval u8 ParseNibble(char c) {
            AMS_ASSUME(('0' <= c && c <= '9') || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'));
            if ('0' <= c && c <= '9') {
                return c - '0';
            } else if ('A' <= c && c <= 'F') {
                return c - 'A' + 0xA;
            } else {
                return c - 'a' + 0xA;
            }
        }
    };

    struct PatternPatch {
        const char *name;
        PatternData pattern;
        s32 instruction_offset;
        s32 patch_offset;
        bool (*condition)(u32 instruction);
        PatchData (*patch)(u32 instruction);
        bool (*applied)(const u8 *mapped, u32 instruction);
        u32 match_index;
        hos::Version min_version;
        hos::Version max_version;
    };

    struct PatchTarget {
        ncm::ProgramId program_id;
        ModuleType module_type;
        bool match_module_id;
        ro::ModuleId module_id;
        const PatternPatch *patterns;
        size_t num_patterns;
        bool requires_usb30_force_enabled;
        hos::Version min_version;
        hos::Version max_version;
    };

    constexpr bool strb_cond(u32 instruction) {
        return (instruction >> 24) == 0x39;
    }

    constexpr bool sub_cond(u32 instruction) {
        return (instruction >> 24) == 0xD1;
    }

    constexpr bool bl_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0x25 || type == 0x94 || type == 0x97;
    }

    constexpr bool tbz_cond(u32 instruction) {
        return ((instruction >> 24) & 0x7F) == 0x36;
    }

    constexpr bool adr_cond(u32 instruction) {
        return (instruction >> 24) == 0x10;
    }

    constexpr bool es_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0xD1 || type == 0xA9 || type == 0xAA || type == 0x2A || type == 0x92;
    }

    constexpr bool ctest_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0xF9 || type == 0xA9 || type == 0xF8;
    }

    constexpr bool block_fw_updates_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0xA8 || type == 0xA9 || type == 0xF8 || type == 0xF9;
    }

    constexpr PatchData mov0_ret_patch_data{"0xE0031F2AC0035FD6"};
    constexpr PatchData nop_patch_data{"0x1F2003D5"};
    constexpr PatchData strb_wzr_x19_patch_data{"0x7F020039"};
    constexpr PatchData mov_w0_1_patch_data{"20008052"};
    constexpr PatchData mov_w0_1_ret_patch_data{"0x20008052C0035FD6"};
    constexpr PatchData ret0_patch_data{"0xE0031F2A"};
    constexpr PatchData mov0_patch_data{"0xE0031FAA"};
    constexpr PatchData mov2_patch_data{"0xE2031FAA"};
    constexpr PatchData ctest_patch_data{"0x00309AD2001EA1F2610100D4E0031FAAC0035FD6"};

    inline PatchData nop_patch(u32) {
        return nop_patch_data;
    }

    inline PatchData strb_wzr_x19_patch(u32) {
        return strb_wzr_x19_patch_data;
    }

    inline PatchData mov_w0_1_ret_patch(u32) {
        return mov_w0_1_ret_patch_data;
    }

    inline PatchData mov_w0_1_patch(u32) {
        return mov_w0_1_patch_data;
    }

    inline PatchData ret0_patch(u32) {
        return ret0_patch_data;
    }

    inline PatchData mov0_ret_patch(u32) {
        return mov0_ret_patch_data;
    }

    inline PatchData mov0_patch(u32) {
        return mov0_patch_data;
    }

    inline PatchData mov2_patch(u32) {
        return mov2_patch_data;
    }

    inline PatchData ctest_patch(u32) {
        return ctest_patch_data;
    }

    inline bool nop_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return nop_patch_data.IsApplied(mapped);
    }

    inline bool strb_wzr_x19_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return strb_wzr_x19_patch_data.IsApplied(mapped);
    }

    inline bool mov_w0_1_ret_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w0_1_ret_patch_data.IsApplied(mapped);
    }

    inline bool mov_w0_1_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w0_1_patch_data.IsApplied(mapped);
    }

    inline bool ret0_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return ret0_patch_data.IsApplied(mapped);
    }

    inline bool mov0_ret_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov0_ret_patch_data.IsApplied(mapped);
    }

    inline bool mov0_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov0_patch_data.IsApplied(mapped);
    }

    inline bool mov2_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov2_patch_data.IsApplied(mapped);
    }

    inline bool ctest_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return ctest_patch_data.IsApplied(mapped);
    }


    /* name, pattern, instruction_offset, patch_offset, condition, patch, applied, match_index, min_version, max_version */

    /* TODO: Remove this if/when a cleaner solution is implemented by hbmenu/libnx. */
    constexpr inline PatternPatch AmPatterns[] = {
        { "am_homebrew_fix_1", PatternData{"0x94......F9......F9........00410491"}, 17, 0, bl_cond, nop_patch, nop_applied, 0, hos::Version_22_0_0, hos::Version_Max },
    };

    constexpr inline PatternPatch NsPatterns[] = {
        { "force_compatability_type_out_value_region_to_global_1", PatternData{"0x35E8134039F4031F..68020039"}, 9, 0, strb_cond, strb_wzr_x19_patch, strb_wzr_x19_applied, 1, hos::Version_9_0_0, hos::Version_Max },
    };

    constexpr inline PatternPatch UsbPatterns[] = {
        { "usb_patch_1", PatternData{"0x019408000012A92B8A52"}, -2, 0, bl_cond, mov_w0_1_patch, mov_w0_1_applied, 0, hos::Version_9_0_0, hos::Version_10_0_0 },
        { "usb_patch_2", PatternData{"0xFFC300D1FD7B02A9FD83009188"}, 0, 0, sub_cond, mov_w0_1_ret_patch, mov_w0_1_ret_applied, 0, hos::Version_11_0_0, hos::Version_Max },
        { "usb_patch_3", PatternData{"0xFFC300D1FD7B02A9FD83009188"}, 0, 0, sub_cond, mov_w0_1_ret_patch, mov_w0_1_ret_applied, 1, hos::Version_11_0_0, hos::Version_Max },
    };

    constexpr inline PatternPatch EsPatterns[] = {
        { "es_1.0.0-8.1.1",     PatternData{"0x0091....0094..7E4092"},                 10, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_1_0_0,  hos::Version_8_1_1  },
        { "es_9.0.0-11.0.1",    PatternData{"0x00..........A0....D1....FF97"},        14, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_9_0_0,  hos::Version_11_0_1 },
        { "es_12.0.0-18.1.0",   PatternData{"0x02........D2..52....0091"},            32, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_12_0_0, hos::Version_18_1_0 },
        { "es_19.0.0-21.2.0",   PatternData{"0xA1........031F2A....0091"},            32, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_19_0_0, hos::Version_21_2_0 },
        { "es_22.0.0+",         PatternData{"0xA0630091....FE97A08300D1....FE97"},    16, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_22_0_0, hos::Version_Max    },
    };

    constexpr inline PatternPatch NifmPatterns[] = {
        { "ctest_1.0.0-19.0.1", PatternData{"0x03..AAE003..AA......39....04F8........E0"}, -29, 0, ctest_cond, ctest_patch, ctest_applied, 0, hos::Version_1_0_0,  hos::Version_19_0_1 },
        { "ctest_20.0.0+",      PatternData{"0x03..AA......AA..................0314AA....14AA"}, -17, 0, ctest_cond, ctest_patch, ctest_applied, 0, hos::Version_20_0_0, hos::Version_Max    },
    };

    constexpr inline PatternPatch NimPatterns[] = {
        { "blankcal0crashfix_17.0.0+",    PatternData{"0x00351F2003D5..............................97....0094....00..........61"}, 6,   0, adr_cond,        mov2_patch,     mov2_applied,     0, hos::Version_17_0_0, hos::Version_Max    },
        { "blockfirmwareupdates_1.0.0-5.1.0", PatternData{"0x1139F3"},                                                       -30, 0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_1_0_0,  hos::Version_5_1_0  },
        { "blockfirmwareupdates_6.0.0-6.2.0", PatternData{"0xF30301AA..4E"},                                                 -40, 0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_6_0_0,  hos::Version_6_2_0  },
        { "blockfirmwareupdates_7.0.0-10.2.0", PatternData{"0xF30301AA014C"},                                                -36, 0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_7_0_0,  hos::Version_10_2_0 },
        { "blockfirmwareupdates_11.0.0-11.0.1", PatternData{"0x9AF0....................C0035FD6"},                           16,  0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_11_0_0, hos::Version_11_0_1 },
        { "blockfirmwareupdates_12.0.0+", PatternData{"0x41....4C............C0035FD6"},                                     14,  0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_12_0_0, hos::Version_Max    },
    };

    constexpr inline PatchTarget PatchTargets[] = {
        /* program_id, module_type, match_module_id, module_id, patterns, num_patterns, requires_usb30_force_enabled, min_version, max_version */
        { ncm::SystemProgramId::Am,   ModuleType::Main, false, {}, AmPatterns,   util::size(AmPatterns),   false, hos::Version_22_0_0, hos::Version_Max },
        { ncm::SystemProgramId::Ns,   ModuleType::Main, false, {}, NsPatterns,   util::size(NsPatterns),   false, hos::Version_9_0_0,  hos::Version_Max },
        { ncm::SystemProgramId::Usb,  ModuleType::Main, false, {}, UsbPatterns,  util::size(UsbPatterns),  true,  hos::Version_9_0_0,  hos::Version_Max },
        { ncm::SystemProgramId::Es,   ModuleType::Main, false, {}, EsPatterns,   util::size(EsPatterns),   false, hos::Version_2_0_0,  hos::Version_Max },
        { ncm::SystemProgramId::Nifm, ModuleType::Main, false, {}, NifmPatterns, util::size(NifmPatterns), false, hos::Version_Min,    hos::Version_Max },
        { ncm::SystemProgramId::Nim,  ModuleType::Main, false, {}, NimPatterns,  util::size(NimPatterns),  false, hos::Version_Min,    hos::Version_Max },
    };

    constexpr std::span<const PatchTarget> GetPatchTargets() {
        return std::span<const PatchTarget>(PatchTargets, util::size(PatchTargets));
    }
}
