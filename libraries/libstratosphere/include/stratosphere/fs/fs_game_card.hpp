/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <stratosphere/fs/fs_common.hpp>
#include <stratosphere/gc/gc.hpp>

namespace ams::fs {

    /* ACCURATE_TO_VERSION: Unknown */
    constexpr inline size_t GameCardCidSize = 0x10;
    constexpr inline size_t GameCardDeviceIdSize = 0x10;

    enum class GameCardPartition {
        Update = 0,
        Normal = 1,
        Secure = 2,
        Logo   = 3,
    };

    enum class GameCardPartitionRaw {
        NormalReadable,
        SecureReadable,
        RootWriteable,
    };

    enum GameCardAttribute : u8 {
        GameCardAttribute_AutoBootFlag                         = (1 << 0),
        GameCardAttribute_HistoryEraseFlag                     = (1 << 1),
        GameCardAttribute_RepairToolFlag                       = (1 << 2),
        GameCardAttribute_DifferentRegionCupToTerraDeviceFlag  = (1 << 3),
        GameCardAttribute_DifferentRegionCupToGlobalDeviceFlag = (1 << 4),

        GameCardAttribute_HasCa10CertificateFlag               = (1 << 7),
    };

    enum class GameCardCompatibilityType : u8 {
        Normal = 0,
        Terra  = 1,

        Global = Normal,
        China  = Terra,
    };

    struct GameCardErrorReportInfo {
        u16 game_card_crc_error_num;
        u8  last_deactivate_reason;
        u8  reserved1;
        u16 asic_crc_error_num;
        u16 reserved2;
        u16 refresh_num;
        u16 reserved3;
        u16 retry_limit_out_num;
        u16 timeout_retry_num;
        u16 asic_reinitialize_failure_detail;
        u16 insertion_count;
        u16 removal_count;
        u16 asic_reinitialize_num;
        u32 initialize_count;
        u16 asic_reinitialize_failure_num;
        u16 awaken_failure_num;
        u16 reserved4;
        u16 refresh_succeeded_count;
        u32 last_read_error_page_address;
        u32 last_read_error_page_count;
        u32 awaken_count;
        u32 read_count_from_insert;
        u32 read_count_from_awaken;
        u32 last_deactivate_reason_result;
        u32 reserved5;
    };
    static_assert(util::is_pod<GameCardErrorReportInfo>::value);
    static_assert(sizeof(GameCardErrorReportInfo) == 0x40);

    struct GameCardErrorInfo {
        u16 game_card_crc_error_num;
        u16 reserved1;
        u16 asic_crc_error_num;
        u16 reserved2;
        u16 refresh_num;
        u16 reserved3;
        u16 retry_limit_out_num;
        u16 timeout_retry_num;
    };
    static_assert(util::is_pod<GameCardErrorInfo>::value);
    static_assert(sizeof(GameCardErrorInfo) == 0x10);

    struct GameCardDetailedErrorReportInfo {
        u8 data[0xA80];
    };
    static_assert(util::is_pod<GameCardDetailedErrorReportInfo>::value);
    static_assert(sizeof(GameCardDetailedErrorReportInfo) == 0xA80);

    struct GameCardUpdatePartitionInfo {
        u32 version;
        u32 reserved;
        u64 id;
    };
    static_assert(util::is_pod<GameCardUpdatePartitionInfo>::value);
    static_assert(sizeof(GameCardUpdatePartitionInfo) == 0x10);

    struct GameCardRmaInformation {
        u8 data[0x200];
    };
    static_assert(util::is_pod<GameCardRmaInformation>::value);
    static_assert(sizeof(GameCardRmaInformation) == 0x200);

    struct GameCardAsicCertificateSet {
        u8 data[0x513];
    };
    static_assert(util::is_pod<GameCardAsicCertificateSet>::value);
    static_assert(sizeof(GameCardAsicCertificateSet) == 0x513);

    using GameCardHandle = u32;

    Result GetGameCardHandle(GameCardHandle *out);
    Result MountGameCardPartition(const char *name, GameCardHandle handle, GameCardPartition partition);

    Result GetGameCardCid(void *dst, size_t size);
    Result GetGameCardIdSet(gc::GameCardIdSet *out);
    Result GetGameCardDeviceId(void *dst, size_t size);
    Result GetGameCardDeviceIdForProdCard(void *dst, size_t size, const void *card_header, size_t card_header_size);
    Result GetGameCardDeviceCertificate(void *dst, size_t size, GameCardHandle handle);
    Result GetGameCardDeviceCertificate(s64 *out_size, void *dst, size_t size, GameCardHandle handle);
    Result GetGameCardAsicInfo(GameCardRmaInformation *out, const void *fw_buffer, size_t fw_buffer_size);
    Result GetGameCardAsicCertificate(GameCardAsicCertificateSet *out);
    Result ChallengeCardExistence(void *dst, size_t size, const void *seed, size_t seed_size, const void *value, size_t value_size, GameCardHandle handle);
    Result GetGameCardImageHash(void *dst, size_t size, GameCardHandle handle);
    Result GetGameCardApplicationIdList(u16 *out_count, void *dst, size_t size, GameCardHandle handle);
    Result GetGameCardCardHeaderSet(void *dst, size_t size, GameCardHandle handle);
    Result GetGameCardAttribute(GameCardAttribute *out, GameCardHandle handle);
    Result GetGameCardCompatibilityType(GameCardCompatibilityType *out, GameCardHandle handle);
    Result GetGameCardUpdatePartitionInfo(bool *out_exists, GameCardUpdatePartitionInfo *out, GameCardHandle handle);

    Result GetGameCardErrorInfo(GameCardErrorInfo *out);
    Result GetGameCardErrorReportInfo(GameCardErrorReportInfo *out);
    Result GetGameCardDetailedErrorReportInfo(GameCardDetailedErrorReportInfo *out);
    Result EraseGameCard(u32 game_card_size, u64 normal_area_size);
    Result ForceEraseGameCard();
    void FinalizeGameCardDriver();
    Result SetGameCardSessionCreationDelay(bool enabled, u32 minimum_delay_micro_seconds, bool forced);
    Result SetGameCardClockRateForSpeedEmulation(s32 clock_rate);
    Result ClearGameCardClockRateForSpeedEmulation();
    Result RegisterGameCardConfigurationData(const void *data, size_t size);

    bool IsGameCardInserted();

}
