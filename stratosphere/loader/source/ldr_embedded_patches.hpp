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
                auto low  = ParseNibble(*(s++));
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

    enum EmbeddedPatchDisableFlag : u32 {
        EmbeddedPatchDisableFlag_None = 0,
        EmbeddedPatchDisableFlag_Es   = (1u << 0),
        EmbeddedPatchDisableFlag_Nifm = (1u << 1),
        EmbeddedPatchDisableFlag_Nim  = (1u << 2),
    };

    struct PatchTarget {
        ncm::ProgramId program_id;
        ModuleType module_type;
        bool match_module_id;
        ro::ModuleId module_id;
        const PatternPatch *patterns;
        size_t num_patterns;
        bool requires_usb30_force_enabled;
        u32 disable_flag;
        hos::Version min_version;
        hos::Version max_version;
    };

    constexpr bool strb_cond(u32 instruction) {
        return (instruction >> 24) == 0x39;
    }

    constexpr bool str_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0xF9 || type == 0xFC || type == 0xF8;
    }

    constexpr bool sub_cond(u32 instruction) {
        return (instruction >> 24) == 0xD1;
    }

    constexpr bool bl_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0x25 || type == 0x94 || type == 0x97;
    }

    constexpr bool blo_cond(u32 instruction) {
        return (instruction >> 24) == 0x54;
    }

    constexpr bool movk_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0x72 || type == 0x92 || type == 0xB2 || type == 0xD2 || type == 0xF2;
    }

    constexpr bool movz_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0x52 || type == 0xD2;
    }

    constexpr bool ldr_cond(u32 instruction) {
        const auto type = instruction >> 24;
        return type == 0xF9 || type == 0xB9 || type == 0xF8;
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

    /* web memory patch 1 */
    constexpr PatchData mov_x9_40000000_patch_data{"0900A8D2"};
    /* web memory patch 2 */
    constexpr PatchData mov_w2_0_patch_data{"02008052"};
    /* web memory patch 3 */
    constexpr PatchData mov_w1_b000000_patch_data{"0160A152"};
    /* web memory patch 4 system/open */
    constexpr PatchData mov_w3_b000000_patch_data{"0360A152"};
    /* web memory patch 2 offline */
    constexpr PatchData mov_w2_b000000_patch_data{"0260A152"};
    /* web memory patch 5 & 6 */
    constexpr PatchData mov_w1_9000000_patch_data{"0120A152"};
    /* web memory patch 3 (20.x offline, covers patch 4) */
    constexpr PatchData mov_w25_aa00000_patch_data{"1954A152"};
    /* web memory patch 3 (20.x web/shop/loginshare/wifiwebauth, covers patch 4) */
    constexpr PatchData mov_w23_aa00000_patch_data{"1754A152"};
    /* web memory patch 6 (20.x) */
    constexpr PatchData mov_w20_9000000_patch_data{"1420A152"};

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

    inline PatchData mov_x9_40000000_patch(u32) {
        return mov_x9_40000000_patch_data;
    }

    inline PatchData mov_w2_0_patch(u32) {
        return mov_w2_0_patch_data;
    }

    inline PatchData mov_w1_b000000_patch(u32) {
        return mov_w1_b000000_patch_data;
    }
    
    inline PatchData mov_w3_b000000_patch(u32) {
        return mov_w3_b000000_patch_data;
    }
    inline PatchData mov_w2_b000000_patch(u32) {
        return mov_w2_b000000_patch_data;
    }
    inline PatchData mov_w1_9000000_patch(u32) {
        return mov_w1_9000000_patch_data;
    }
    inline PatchData mov_w25_aa00000_patch(u32) {
        return mov_w25_aa00000_patch_data;
    }
    inline PatchData mov_w23_aa00000_patch(u32) {
        return mov_w23_aa00000_patch_data;
    }
    inline PatchData mov_w20_9000000_patch(u32) {
        return mov_w20_9000000_patch_data;
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

    inline bool mov_x9_40000000_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_x9_40000000_patch_data.IsApplied(mapped);
    }

    inline bool mov_w2_0_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w2_0_patch_data.IsApplied(mapped);
    }

    inline bool mov_w1_b000000_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w1_b000000_patch_data.IsApplied(mapped);
    }

    inline bool mov_w3_b000000_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w3_b000000_patch_data.IsApplied(mapped);
    }

    inline bool mov_w2_b000000_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w2_b000000_patch_data.IsApplied(mapped);
    }

    inline bool mov_w1_9000000_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w1_9000000_patch_data.IsApplied(mapped);
    }

    inline bool mov_w25_aa00000_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w25_aa00000_patch_data.IsApplied(mapped);
    }

    inline bool mov_w23_aa00000_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w23_aa00000_patch_data.IsApplied(mapped);
    }

    inline bool mov_w20_9000000_applied(const u8 *mapped, u32 instruction) {
        AMS_UNUSED(instruction);
        return mov_w20_9000000_patch_data.IsApplied(mapped);
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
    /* This patches the teardown function calls within AM to NOP */
    constexpr inline PatternPatch AmPatterns[] = {
        { "am_disable_teardown_1", PatternData{"0x682646391F0500716100005460420691794DFF97"}, 16, 0, bl_cond, nop_patch, nop_applied, 0, hos::Version_22_0_0, hos::Version_22_0_0 },
    };

    /* This patch enables Chinese region cartridges to run on Global-firmware consoles */
    /* This patch is applied to nn::ns::detail::IApplicationManagerInterface - command 510, GetGameCardPlatformRegion. */
    /* This patch targets the byte GetGameCardPlatformRegion fetches from nn::fssrv::sf::IDeviceOperator - command 220, GetGameCardCompatibilityType */
    /* This patch forces it's value to become 0, which is the Global-firmware region flag */
    constexpr inline PatternPatch NsPatterns[] = {
        { "force_compatability_type_out_value_region_to_global_1", PatternData{"0x35E8134039F4031F..68020039"}, 9, 0, strb_cond, strb_wzr_x19_patch, strb_wzr_x19_applied, 1, hos::Version_9_0_0, hos::Version_Max },
    };

    /* These are the patches that enable usb 3.0 speeds */
    constexpr inline PatternPatch UsbPatterns[] = {
        { "usb_patch_1", PatternData{"0x019408000012A92B8A52"}, -2, 0, bl_cond, mov_w0_1_patch, mov_w0_1_applied, 0, hos::Version_9_0_0, hos::Version_10_0_0 },
        { "usb_patch_2", PatternData{"0xFFC300D1FD7B02A9FD83009188"}, 0, 0, sub_cond, mov_w0_1_ret_patch, mov_w0_1_ret_applied, 0, hos::Version_11_0_0, hos::Version_Max },
        { "usb_patch_3", PatternData{"0xFFC300D1FD7B02A9FD83009188"}, 0, 0, sub_cond, mov_w0_1_ret_patch, mov_w0_1_ret_applied, 1, hos::Version_11_0_0, hos::Version_Max },
    };

    /* The following patches reduce memory usage by the browser applets: */
    /*   OfflineWeb (LibAppletOff): 20.0.0+ */
    /*   SystemWeb, OpenWeb: 22.0.0+ (these applets were added in 22.0.0) */
    /*   Web, Shop, LoginShare, WifiWebAuth (LibAppletWeb/Shop/Lns/Auth): 20.0.0 - 21.2.0 (stubs from 22.0.0+) */
    /* Breakdown of each patch in order: */
    /* #1 - Set TotalAvailableMemorySize to 1GB. */
    /* #2 - Change the root heap "Reserved" region allocation size to 0 (from 0xF7E000 on 20.x, 0x47E000 on 21.0.0-22.0.0, 0x43E000 on 22.5.0+). */
    /* #3 - Patch the initial memory block allocation size (from 0xAE00000 to 0xAA00000 on 20.x, from 0xB400000 to 0xB000000 on 21.0.0+). */
    /* #4 - Patch the initial memory block heap size from 0xB400000 to 0xB000000 (21.0.0+) */
    /* #5 - Patch the browser engine memory capacity from 0xAA00000 to 0x9000000. */
    /* #6 - Patch the browser engine memory allocation size from 0xAA00000 to 0x9000000. */

    /* Why 20.x only has 5 patches: */
    /* On 20.x the initial memory block size is loaded once into a callee-saved register (mov w25, #0xae00000 on */
    /* OfflineWeb, mov w23, #0xae00000 on Web/Shop/LoginShare/WifiWebAuth), which is then passed both to the block */
    /* allocation and to the heap initialization. Patching that one instruction changes both sizes, so #3 also */
    /* covers #4 and there is no separate #4. */
    /* From 21.0.0 onward, each call loads the size separately (mov w1, #0xb400000 for the allocation, then */
    /* mov w2, #0xb400000 for the heap size on OfflineWeb, or mov w3, #0xb400000 on SystemWeb, OpenWeb and */
    /* Web/Shop/LoginShare/WifiWebAuth), so #3 and #4 have to be patched separately. */

    /* On 20.x, #3 shrinks the block by the same 0x400000 as 21.0.0+ does. On OfflineWeb 20.0.0, with "Reserved" */
    /* set to 0, the child heaps need 0x9E00000, so the patched 0xAA00000 leaves 0xC00000 spare (0x1C0000 on 23.0.0). */

    /* On 20.x, #6 patches mov w20, #0xaa00000. That register is used both for the engine memory allocation and for */
    /* the size of the heap created on it, so one instruction covers both. The allocation reduces the capacity set */
    /* by #5 by the requested size, so patching #5 and #6 to the same value keeps them consistent. This is the */
    /* closest 20.x equivalent of the 21.0.0+ allocation; it is not yet confirmed on hardware. */

    /* Web/Shop/LoginShare/WifiWebAuth are built with return-address hardening and different register allocation, */
    /* so they need their own patterns for #3-#6, even though the patched instructions and values are the same. */

    constexpr inline PatternPatch WebOfflinePatterns[] = {
        /* #2 matches on every supported version */
        { "patch_2",      PatternData{"0x02009C52....A072"},                 4,  0, movk_cond, mov_w2_0_patch,         mov_w2_0_applied,         0, hos::Version_20_0_0, hos::Version_Max    },

        /* 20.x */
        { "patch_1_20.x", PatternData{"0xA1028052E0230091C2FF8F12E3031FAA"}, 36, 0, ldr_cond,  mov_x9_40000000_patch,  mov_x9_40000000_applied,  0, hos::Version_20_0_0, hos::Version_20_5_0 },
        { "patch_3_20.x", PatternData{"0x195CA152E00318AAE10319AA"},         0,  0, movz_cond, mov_w25_aa00000_patch,  mov_w25_aa00000_applied,  0, hos::Version_20_0_0, hos::Version_20_5_0 },
        { "patch_5_20.x", PatternData{"0x0154A152E00317AA"},                 0,  0, movz_cond, mov_w1_9000000_patch,   mov_w1_9000000_applied,   0, hos::Version_20_0_0, hos::Version_20_5_0 },
        { "patch_6_20.x", PatternData{"0x1454A152E10314AA"},                 0,  0, movz_cond, mov_w20_9000000_patch,  mov_w20_9000000_applied,  0, hos::Version_20_0_0, hos::Version_20_5_0 },

        /* 21.0.0+ */
        { "patch_1",      PatternData{"0xA1028052C2FF8F12E3031FAA"},         32, 0, ldr_cond,  mov_x9_40000000_patch,  mov_x9_40000000_applied,  0, hos::Version_21_0_0, hos::Version_Max    },
        { "patch_3",      PatternData{"0x0168A152"},                         0,  0, movz_cond, mov_w1_b000000_patch,   mov_w1_b000000_applied,   0, hos::Version_21_0_0, hos::Version_Max    },
        { "patch_4",      PatternData{"0xE10300AA60....91..68A152"},         8,  0, movz_cond, mov_w2_b000000_patch,   mov_w2_b000000_applied,   0, hos::Version_21_0_0, hos::Version_Max    },
        { "patch_5",      PatternData{"0xB5......910154A152"},               5,  0, movz_cond, mov_w1_9000000_patch,   mov_w1_9000000_applied,   0, hos::Version_21_0_0, hos::Version_Max    },
        { "patch_6",      PatternData{"0xA90154A152"},                       1,  0, movz_cond, mov_w1_9000000_patch,   mov_w1_9000000_applied,   0, hos::Version_21_0_0, hos::Version_Max    },
    };

    constexpr inline PatternPatch WebSystemOpenPatterns[] = {
        { "patch_1",      PatternData{"0xA1028052C2FF8F12E3031FAA"},         32, 0, ldr_cond,  mov_x9_40000000_patch,  mov_x9_40000000_applied,  0, hos::Version_22_0_0, hos::Version_Max    },
        { "patch_2",      PatternData{"0x02009C52....A072"},                 4,  0, movk_cond, mov_w2_0_patch,         mov_w2_0_applied,         0, hos::Version_22_0_0, hos::Version_Max    },
        { "patch_3",      PatternData{"0x0168A152"},                         0,  0, movz_cond, mov_w1_b000000_patch,   mov_w1_b000000_applied,   0, hos::Version_22_0_0, hos::Version_Max    },
        { "patch_4",      PatternData{"0x60820691..68A152"},                 4,  0, movz_cond, mov_w3_b000000_patch,   mov_w3_b000000_applied,   0, hos::Version_22_0_0, hos::Version_Max    },
        { "patch_5",      PatternData{"0xB5......910154A152"},               5,  0, movz_cond, mov_w1_9000000_patch,   mov_w1_9000000_applied,   0, hos::Version_22_0_0, hos::Version_Max    },
        { "patch_6",      PatternData{"0xA90154A152"},                       1,  0, movz_cond, mov_w1_9000000_patch,   mov_w1_9000000_applied,   0, hos::Version_22_0_0, hos::Version_Max    },
    };

    /* Web, Shop, LoginShare, WifiWebAuth */
    constexpr inline PatternPatch WebLibraryAppletPatterns[] = {
        /* #2 matches on every supported version */
        { "patch_2",      PatternData{"0x02009C52....A072"},                 4,  0, movk_cond, mov_w2_0_patch,         mov_w2_0_applied,         0, hos::Version_20_0_0, hos::Version_21_2_0 },

        /* 20.x */
        { "patch_1_20.x", PatternData{"0xA1028052E0230091C2FF8F12E3031FAA"}, 36, 0, ldr_cond,  mov_x9_40000000_patch,  mov_x9_40000000_applied,  0, hos::Version_20_0_0, hos::Version_20_5_0 },
        { "patch_3_20.x", PatternData{"0x175CA152E00316AAE10317AA"},         0,  0, movz_cond, mov_w23_aa00000_patch,  mov_w23_aa00000_applied,  0, hos::Version_20_0_0, hos::Version_20_5_0 },
        { "patch_5_20.x", PatternData{"0x0154A152E0031CAA"},                 0,  0, movz_cond, mov_w1_9000000_patch,   mov_w1_9000000_applied,   0, hos::Version_20_0_0, hos::Version_20_5_0 },
        { "patch_6_20.x", PatternData{"0x1454A152........F30300AAE10314AA"}, 0,  0, movz_cond, mov_w20_9000000_patch,  mov_w20_9000000_applied,  0, hos::Version_20_0_0, hos::Version_20_5_0 },

        /* 21.x */
        { "patch_1",      PatternData{"0xA1028052C2FF8F12E3031FAA"},         32, 0, ldr_cond,  mov_x9_40000000_patch,  mov_x9_40000000_applied,  0, hos::Version_21_0_0, hos::Version_21_2_0 },
        { "patch_3",      PatternData{"0x0168A152"},                         0,  0, movz_cond, mov_w1_b000000_patch,   mov_w1_b000000_applied,   0, hos::Version_21_0_0, hos::Version_21_2_0 },
        { "patch_4",      PatternData{"0xE20300AA........60....910368A152"}, 12, 0, movz_cond, mov_w3_b000000_patch,   mov_w3_b000000_applied,   0, hos::Version_21_0_0, hos::Version_21_2_0 },
        { "patch_5",      PatternData{"0xB5......910154A152"},               5,  0, movz_cond, mov_w1_9000000_patch,   mov_w1_9000000_applied,   0, hos::Version_21_0_0, hos::Version_21_2_0 },
        { "patch_6",      PatternData{"0xA90154A152"},                       1,  0, movz_cond, mov_w1_9000000_patch,   mov_w1_9000000_applied,   0, hos::Version_21_0_0, hos::Version_21_2_0 },
    };

    /* disable verification requirement on the common ticketbin */
    constexpr inline PatternPatch EsPatterns[] = {
        { "es_1.0.0-8.1.1",     PatternData{"0x0091....0094..7E4092"},                10, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_1_0_0,  hos::Version_8_1_1  },
        { "es_9.0.0-11.0.1",    PatternData{"0x00..........A0....D1....FF97"},        14, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_9_0_0,  hos::Version_11_0_1 },
        { "es_12.0.0-18.1.0",   PatternData{"0x02........D2..52....0091"},            32, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_12_0_0, hos::Version_18_1_0 },
        { "es_19.0.0-21.2.0",   PatternData{"0xA1........031F2A....0091"},            32, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_19_0_0, hos::Version_21_2_0 },
        { "es_22.0.0+",         PatternData{"0xA0630091....FE97A08300D1....FE97"},    16, 0, es_cond, mov0_patch, mov0_applied, 0, hos::Version_22_0_0, hos::Version_Max    },
    };

    /* force connectivity open despite not receiving "X-Organizarion: Nintendo" header from http ctest call */
    constexpr inline PatternPatch NifmPatterns[] = {
        { "ctest_1.0.0-19.0.1", PatternData{"0x03..AAE003..AA......39....04F8........E0"},       -29, 0, ctest_cond, ctest_patch, ctest_applied, 0, hos::Version_1_0_0,  hos::Version_19_0_1 },
        { "ctest_20.0.0+",      PatternData{"0x03..AA......AA..................0314AA....14AA"}, -17, 0, ctest_cond, ctest_patch, ctest_applied, 0, hos::Version_20_0_0, hos::Version_Max    },
    };

    /* Prevents crashes from broken ssl certificate("prodinfo"), and blocks automatic firmware upgrades */
    constexpr inline PatternPatch NimPatterns[] = {
        { "blankcal0crashfix_17.0.0+",    PatternData{"0x00351F2003D5..............................97....0094....00..........61"}, 6,   0, adr_cond,        mov2_patch,     mov2_applied,     0, hos::Version_17_0_0, hos::Version_Max    },
        { "blockfirmwareupdates_1.0.0-5.1.0", PatternData{"0x1139F3"},                                                       -30, 0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_1_0_0,  hos::Version_5_1_0  },
        { "blockfirmwareupdates_6.0.0-6.2.0", PatternData{"0xF30301AA..4E"},                                                 -40, 0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_6_0_0,  hos::Version_6_2_0  },
        { "blockfirmwareupdates_7.0.0-10.2.0", PatternData{"0xF30301AA014C"},                                                -36, 0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_7_0_0,  hos::Version_10_2_0 },
        { "blockfirmwareupdates_11.0.0-11.0.1", PatternData{"0x9AF0....................C0035FD6"},                           16,  0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_11_0_0, hos::Version_11_0_1 },
        { "blockfirmwareupdates_12.0.0+", PatternData{"0x41....4C............C0035FD6"},                                     14,  0, block_fw_updates_cond, mov0_ret_patch, mov0_ret_applied, 0, hos::Version_12_0_0, hos::Version_Max    },
    };

    constexpr inline PatchTarget PatchTargets[] = {
        /* program_id, module_type, match_module_id, module_id, patterns, num_patterns, requires_usb30_force_enabled, disable_flag, min_version, max_version */
        { ncm::SystemProgramId::Am,        ModuleType::Main, false, {}, AmPatterns,   util::size(AmPatterns),                     false, EmbeddedPatchDisableFlag_None, hos::Version_22_0_0, hos::Version_Max },
        { ncm::SystemProgramId::Ns,        ModuleType::Main, false, {}, NsPatterns,   util::size(NsPatterns),                     false, EmbeddedPatchDisableFlag_None, hos::Version_9_0_0,  hos::Version_Max },
        { ncm::SystemProgramId::Usb,       ModuleType::Main, false, {}, UsbPatterns,  util::size(UsbPatterns),                    true,  EmbeddedPatchDisableFlag_None, hos::Version_9_0_0,  hos::Version_Max },
        { ncm::SystemProgramId::Es,        ModuleType::Main, false, {}, EsPatterns,   util::size(EsPatterns),                     false, EmbeddedPatchDisableFlag_Es,   hos::Version_2_0_0,  hos::Version_Max },
        { ncm::SystemProgramId::Nifm,      ModuleType::Main, false, {}, NifmPatterns, util::size(NifmPatterns),                   false, EmbeddedPatchDisableFlag_Nifm, hos::Version_Min,    hos::Version_Max },
        { ncm::SystemProgramId::Nim,       ModuleType::Main, false, {}, NimPatterns,  util::size(NimPatterns),                    false, EmbeddedPatchDisableFlag_Nim,  hos::Version_Min,    hos::Version_Max },
        /* these are applets */
        { ncm::SystemAppletId::OfflineWeb,  ModuleType::Main, false, {}, WebOfflinePatterns,       util::size(WebOfflinePatterns),       false, EmbeddedPatchDisableFlag_None, hos::Version_20_0_0, hos::Version_Max    },
        { ncm::SystemAppletId::SystemWeb,   ModuleType::Main, false, {}, WebSystemOpenPatterns,    util::size(WebSystemOpenPatterns),    false, EmbeddedPatchDisableFlag_None, hos::Version_22_0_0, hos::Version_Max    },
        { ncm::SystemAppletId::OpenWeb,     ModuleType::Main, false, {}, WebSystemOpenPatterns,    util::size(WebSystemOpenPatterns),    false, EmbeddedPatchDisableFlag_None, hos::Version_22_0_0, hos::Version_Max    },
        { ncm::SystemAppletId::Web,         ModuleType::Main, false, {}, WebLibraryAppletPatterns, util::size(WebLibraryAppletPatterns), false, EmbeddedPatchDisableFlag_None, hos::Version_20_0_0, hos::Version_21_2_0 },
        { ncm::SystemAppletId::Shop,        ModuleType::Main, false, {}, WebLibraryAppletPatterns, util::size(WebLibraryAppletPatterns), false, EmbeddedPatchDisableFlag_None, hos::Version_20_0_0, hos::Version_21_2_0 },
        { ncm::SystemAppletId::LoginShare,  ModuleType::Main, false, {}, WebLibraryAppletPatterns, util::size(WebLibraryAppletPatterns), false, EmbeddedPatchDisableFlag_None, hos::Version_20_0_0, hos::Version_21_2_0 },
        { ncm::SystemAppletId::WifiWebAuth, ModuleType::Main, false, {}, WebLibraryAppletPatterns, util::size(WebLibraryAppletPatterns), false, EmbeddedPatchDisableFlag_None, hos::Version_20_0_0, hos::Version_21_2_0 },

    };

    constexpr std::span<const PatchTarget> GetPatchTargets() {
        return std::span<const PatchTarget>(PatchTargets, util::size(PatchTargets));
    }
}
