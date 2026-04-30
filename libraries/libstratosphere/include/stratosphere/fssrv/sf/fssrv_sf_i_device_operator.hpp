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
#include <vapours.hpp>
#include <stratosphere/sf.hpp>
#include <stratosphere/fs/fs_error_info.hpp>
#include <stratosphere/fs/fs_game_card.hpp>
#include <stratosphere/fs/fs_mmc.hpp>
#include <stratosphere/fs/fs_sd_card.hpp>
#include <stratosphere/fs/fs_speed_emulation.hpp>

namespace ams::fssrv::sf {

    struct DeviceOperatorHostControllerStatus {
        u8 data[0xC];
    };
    static_assert(util::is_pod<DeviceOperatorHostControllerStatus>::value);
    static_assert(sizeof(DeviceOperatorHostControllerStatus) == 0xC);

    struct DeviceOperatorGameCardUpdatePartitionInfo {
        u32 version;
        u32 reserved;
        u64 id;
    };
    static_assert(util::is_pod<DeviceOperatorGameCardUpdatePartitionInfo>::value);
    static_assert(sizeof(DeviceOperatorGameCardUpdatePartitionInfo) == 0x10);

    struct DeviceOperatorGameCardRmaInformation {
        u8 data[0x200];
    };
    static_assert(util::is_pod<DeviceOperatorGameCardRmaInformation>::value);
    static_assert(sizeof(DeviceOperatorGameCardRmaInformation) == 0x200);

    struct DeviceOperatorSdCardInfo {
        u8 data[0x100];
    };
    static_assert(util::is_pod<DeviceOperatorSdCardInfo>::value);
    static_assert(sizeof(DeviceOperatorSdCardInfo) == 0x100);

    struct DeviceOperatorGameCardIdSet {
        u32 id1;
        u32 id2;
        u32 id3;
    };
    static_assert(util::is_pod<DeviceOperatorGameCardIdSet>::value);
    static_assert(sizeof(DeviceOperatorGameCardIdSet) == 0xC);

    struct DeviceOperatorGameCardSizeAndHandle {
        s64 size;
        u32 handle;
        u32 reserved;
    };
    static_assert(util::is_pod<DeviceOperatorGameCardSizeAndHandle>::value);
    static_assert(sizeof(DeviceOperatorGameCardSizeAndHandle) == 0x10);

    struct DeviceOperatorGameCardAsicCertificateSet {
        u8 data[0x513];
    };
    static_assert(util::is_pod<DeviceOperatorGameCardAsicCertificateSet>::value);
    static_assert(sizeof(DeviceOperatorGameCardAsicCertificateSet) == 0x513);

    struct DeviceOperatorGameCardDetailedErrorReportInfo {
        u8 data[0xA80];
    };
    static_assert(util::is_pod<DeviceOperatorGameCardDetailedErrorReportInfo>::value);
    static_assert(sizeof(DeviceOperatorGameCardDetailedErrorReportInfo) == 0xA80);

    struct DeviceOperatorGameCardErrorInfo {
        u16 game_card_crc_error_num;
        u16 reserved1;
        u16 asic_crc_error_num;
        u16 reserved2;
        u16 refresh_num;
        u16 reserved3;
        u16 retry_limit_out_num;
        u16 timeout_retry_num;
    };
    static_assert(util::is_pod<DeviceOperatorGameCardErrorInfo>::value);
    static_assert(sizeof(DeviceOperatorGameCardErrorInfo) == 0x10);

    struct DeviceOperatorGameCardSessionCreationDelay {
        u8 enabled;
        u8 forced;
        u8 reserved[2];
        u32 minimum_delay_micro_seconds;
    };
    static_assert(util::is_pod<DeviceOperatorGameCardSessionCreationDelay>::value);
    static_assert(sizeof(DeviceOperatorGameCardSessionCreationDelay) == 0x8);

    struct DeviceOperatorDeviceSimulationEvent {
        u32 device_type;
        u32 target_operation;
        u32 access_failure_event_type;
        u32 result;
        u8 enabled;
        u8 reserved[3];
    };
    static_assert(util::is_pod<DeviceOperatorDeviceSimulationEvent>::value);
    static_assert(sizeof(DeviceOperatorDeviceSimulationEvent) == 0x14);

}

/* TODO */
/* ACCURATE_TO_VERSION: 20.0.0.0 */
#define AMS_FSSRV_I_DEVICE_OPERATOR_INTERFACE_INFO(C, H) \
    AMS_SF_METHOD_INFO(C, H,   0, Result, IsSdCardInserted,                           (ams::sf::Out<bool> out),                                                                                               (out))                                                         \
    AMS_SF_METHOD_INFO(C, H,   1, Result, GetSdCardSpeedMode,                         (ams::sf::Out<s64> out),                                                                                                (out))                                                         \
    AMS_SF_METHOD_INFO(C, H,   2, Result, GetSdCardCid,                               (ams::sf::OutBuffer out, s64 size),                                                                                     (out, size))                                                   \
    AMS_SF_METHOD_INFO(C, H,   3, Result, GetSdCardUserAreaSize,                      (ams::sf::Out<s64> out),                                                                                                (out))                                                         \
    AMS_SF_METHOD_INFO(C, H,   4, Result, GetSdCardProtectedAreaSize,                 (ams::sf::Out<s64> out),                                                                                                (out))                                                         \
    AMS_SF_METHOD_INFO(C, H,   5, Result, GetAndClearSdCardErrorInfo,                 (ams::sf::Out<ams::fs::StorageErrorInfo> out_sei, ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buf, s64 size),    (out_sei, out_size, out_buf, size))                            \
    AMS_SF_METHOD_INFO(C, H,   6, Result, GetSdCardHostControllerStatus,              (ams::sf::Out<ams::fs::SdCardHostControllerStatus> out),                                                                (out))                                                         \
    AMS_SF_METHOD_INFO(C, H,   7, Result, SetSdCardActivationMode,                    (u8 mode),                                                                                                              (mode))                                                        \
    AMS_SF_METHOD_INFO(C, H,   8, Result, TryGetSdCardInfo,                           (ams::sf::OutBuffer out, s64 size),                                                                                     (out, size))                                                   \
    AMS_SF_METHOD_INFO(C, H, 100, Result, GetMmcCid,                                  (ams::sf::OutBuffer out, s64 size),                                                                                     (out, size))                                                   \
    AMS_SF_METHOD_INFO(C, H, 101, Result, GetMmcSpeedMode,                            (ams::sf::Out<s64> out),                                                                                                (out))                                                         \
    AMS_SF_METHOD_INFO(C, H, 110, Result, EraseMmc,                                   (u32 partition),                                                                                                        (partition))                                                   \
    AMS_SF_METHOD_INFO(C, H, 111, Result, GetMmcPartitionSize,                        (ams::sf::Out<u64> out, u32 partition),                                                                                 (out, partition))                                              \
    AMS_SF_METHOD_INFO(C, H, 112, Result, GetMmcPatrolCount,                          (ams::sf::Out<u32> out),                                                                                                (out))                                                         \
    AMS_SF_METHOD_INFO(C, H, 113, Result, GetAndClearMmcErrorInfo,                    (ams::sf::Out<ams::fs::StorageErrorInfo> out_sei, ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buf, s64 size),    (out_sei, out_size, out_buf, size))                            \
    AMS_SF_METHOD_INFO(C, H, 114, Result, GetMmcExtendedCsd,                          (ams::sf::OutBuffer out, s64 size),                                                                                     (out, size))                                                   \
    AMS_SF_METHOD_INFO(C, H, 115, Result, SuspendMmcPatrol,                           (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 116, Result, ResumeMmcPatrol,                            (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 117, Result, EraseMmcWithRange,                          (u64 offset, u64 size, u64 end),                                                                                        (offset, size, end))                                           \
    AMS_SF_METHOD_INFO(C, H, 118, Result, MarkBeforeEraseMmcPartitionUserData,        (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 119, Result, CheckAfterEraseMmcPartitionUserData,        (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 200, Result, IsGameCardInserted,                         (ams::sf::Out<bool> out),                                                                                               (out))                                                         \
    AMS_SF_METHOD_INFO(C, H, 201, Result, EraseGameCard,                              (u32 gc_size, u64 gc_handle),                                                                                           (gc_size, gc_handle))                                          \
    AMS_SF_METHOD_INFO(C, H, 202, Result, GetGameCardHandle,                          (ams::sf::Out<u32> out),                                                                                                (out))                                                         \
    AMS_SF_METHOD_INFO(C, H, 203, Result, GetGameCardUpdatePartitionInfo,             (ams::sf::Out<u32> out_version, ams::sf::Out<u64> out_id, u32 handle),                                                  (out_version, out_id, handle))                                 \
    AMS_SF_METHOD_INFO(C, H, 204, Result, FinalizeGameCardDriver,                     (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 205, Result, GetGameCardAttribute,                       (ams::sf::Out<u8> out, u32 handle),                                                                                     (out, handle))                                                 \
    AMS_SF_METHOD_INFO(C, H, 206, Result, GetGameCardDeviceCertificate,               (ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buf, s64 buf_size, u32 handle),                                     (out_size, out_buf, buf_size, handle))                         \
    AMS_SF_METHOD_INFO(C, H, 207, Result, GetGameCardAsicInfo,                        (ams::sf::OutBuffer out, s64 out_size, ams::sf::InBuffer in_buf, s64 in_size),                                          (out, out_size, in_buf, in_size))                              \
    AMS_SF_METHOD_INFO(C, H, 208, Result, GetGameCardIdSet,                           (ams::sf::OutBuffer out, s64 size),                                                                                     (out, size))                                                   \
    AMS_SF_METHOD_INFO(C, H, 209, Result, WriteToGameCardDirectly,                    (u64 offset, ams::sf::OutBuffer buf),                                                                                   (offset, buf))                                                 \
    AMS_SF_METHOD_INFO(C, H, 210, Result, SetVerifyWriteEnableFlag,                   (bool flag),                                                                                                            (flag))                                                        \
    AMS_SF_METHOD_INFO(C, H, 211, Result, GetGameCardImageHash,                       (ams::sf::OutBuffer out_hash, u32 handle),                                                                              (out_hash, handle))                                            \
    AMS_SF_METHOD_INFO(C, H, 212, Result, GetGameCardDeviceIdForProdCard,             (ams::sf::OutBuffer out_buf, s64 out_size, ams::sf::InBuffer in_buf, s64 in_size),                                      (out_buf, out_size, in_buf, in_size))                          \
    AMS_SF_METHOD_INFO(C, H, 213, Result, EraseAndWriteParamDirectly,                 (ams::sf::InBuffer in_buf, s64 in_size),                                                                                (in_buf, in_size))                                             \
    AMS_SF_METHOD_INFO(C, H, 214, Result, ReadParamDirectly,                          (ams::sf::OutBuffer out_buf, s64 out_size),                                                                             (out_buf, out_size))                                           \
    AMS_SF_METHOD_INFO(C, H, 215, Result, ForceEraseGameCard,                         (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 216, Result, GetGameCardErrorInfo,                       (ams::sf::Out<ams::fs::GameCardErrorInfo> out),                                                                         (out))                                                         \
    AMS_SF_METHOD_INFO(C, H, 217, Result, GetGameCardErrorReportInfo,                 (ams::sf::Out<ams::fs::GameCardErrorReportInfo> out),                                                                   (out))                                                         \
    AMS_SF_METHOD_INFO(C, H, 218, Result, GetGameCardDeviceId,                        (ams::sf::OutBuffer out, s64 size),                                                                                     (out, size))                                                   \
    AMS_SF_METHOD_INFO(C, H, 219, Result, ChallengeCardExistence,                     (ams::sf::OutBuffer dst, ams::sf::InBuffer seed, ams::sf::InBuffer value, u32 handle),                                  (dst, seed, value, handle))                                    \
    AMS_SF_METHOD_INFO(C, H, 220, Result, GetGameCardCompatibilityType,               (ams::sf::Out<u8> out, u32 handle),                                                                                     (out, handle))                                                 \
    AMS_SF_METHOD_INFO(C, H, 221, Result, GetGameCardAsicCertificate,                 (ams::sf::OutBuffer out, s64 out_size),                                                                                 (out, out_size))                                               \
    AMS_SF_METHOD_INFO(C, H, 222, Result, GetGameCardCardHeaderSet,                   (ams::sf::OutBuffer out_buf, s64 out_size, u32 handle),                                                                 (out_buf, out_size, handle))                                   \
    AMS_SF_METHOD_INFO(C, H, 223, Result, SetGameCardSessionCreationDelay,            (bool enabled, u32 delay, bool flag),                                                                                   (enabled, delay, flag))                                        \
    AMS_SF_METHOD_INFO(C, H, 224, Result, GetGameCardApplicationIdList,               (ams::sf::Out<u16> out_count, ams::sf::OutBuffer out_buf, s64 out_size, u32 handle),                                    (out_count, out_buf, out_size, handle))                        \
    AMS_SF_METHOD_INFO(C, H, 225, Result, RegisterGameCardConfigurationData,          (ams::sf::InBuffer in_buf, s64 in_size),                                                                                (in_buf, in_size))                                             \
    AMS_SF_METHOD_INFO(C, H, 226, Result, GetGameCardDetailedErrorReportInfo,         (ams::sf::OutBuffer out, s64 out_size),                                                                                 (out, out_size))                                               \
    AMS_SF_METHOD_INFO(C, H, 300, Result, SetSpeedEmulationMode,                      (u32 mode),                                                                                                             (mode))                                                        \
    AMS_SF_METHOD_INFO(C, H, 301, Result, GetSpeedEmulationMode,                      (ams::sf::Out<u32> out),                                                                                                (out))                                                         \
    AMS_SF_METHOD_INFO(C, H, 302, Result, SetApplicationStorageSpeed,                 (u32 speed),                                                                                                            (speed))                                                       \
    AMS_SF_METHOD_INFO(C, H, 303, Result, SetGameCardClockRateForSpeedEmulation,      (u32 clock_rate),                                                                                                       (clock_rate))                                                  \
    AMS_SF_METHOD_INFO(C, H, 304, Result, ClearGameCardClockRateForSpeedEmulation,    (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 400, Result, SuspendSdmmcControl,                        (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 401, Result, ResumeSdmmcControl,                         (),                                                                                                                     ())                                                            \
    AMS_SF_METHOD_INFO(C, H, 402, Result, GetSdmmcConnectionStatus,                   (ams::sf::Out<u32> out_speed_mode, ams::sf::Out<u32> out_bus_width, u32 port),                                          (out_speed_mode, out_bus_width, port))                         \
    AMS_SF_METHOD_INFO(C, H, 500, Result, SetDeviceSimulationEvent,                   (u32 device_type, u32 target_op, u32 failure_type, u32 result_value, u8 flag),                                          (device_type, target_op, failure_type, result_value, flag))    \
    AMS_SF_METHOD_INFO(C, H, 501, Result, ClearDeviceSimulationEvent,                 (u32 device_type),                                                                                                      (device_type))

AMS_SF_DEFINE_INTERFACE(ams::fssrv::sf, IDeviceOperator, AMS_FSSRV_I_DEVICE_OPERATOR_INTERFACE_INFO, 0x1484E21C)
