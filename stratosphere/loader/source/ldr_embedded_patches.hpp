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

    constexpr PatchData nop_patch_data{"0x1F2003D5"};
    constexpr PatchData strb_wzr_x19_patch_data{"0x7F020039"};
    constexpr PatchData mov_w0_1_patch_data{"20008052"};
    constexpr PatchData mov_w0_1_ret_patch_data{"0x20008052C0035FD6"};

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

    constexpr inline PatchTarget PatchTargets[] = {
        /* program_id, module_type, match_module_id, module_id, patterns, num_patterns, requires_usb30_force_enabled, min_version, max_version */
        { ncm::SystemProgramId::Am,   ModuleType::Main, false, {}, AmPatterns,   util::size(AmPatterns),  false, hos::Version_22_0_0, hos::Version_Max },
        { ncm::SystemProgramId::Ns,   ModuleType::Main, false, {}, NsPatterns,   util::size(NsPatterns),  false, hos::Version_9_0_0,  hos::Version_Max },
        { ncm::SystemProgramId::Usb,  ModuleType::Main, false, {}, UsbPatterns,  util::size(UsbPatterns), true,  hos::Version_9_0_0,  hos::Version_Max },
    };

    constexpr std::span<const PatchTarget> GetPatchTargets() {
        return std::span<const PatchTarget>(PatchTargets, util::size(PatchTargets));
    }
}
