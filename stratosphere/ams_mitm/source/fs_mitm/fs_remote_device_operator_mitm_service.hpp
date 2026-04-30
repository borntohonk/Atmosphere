#pragma once
#include <stratosphere.hpp>
#include <stratosphere/fssrv/fssrv_interface_adapters.hpp>
#include <stratosphere/fssrv/sf/fssrv_sf_i_device_operator.hpp>

#define AMS_FS_MITM_I_DEVICE_OPERATOR_INTERFACE_INFO(C, H) \
    AMS_SF_METHOD_INFO(C, H,   0, Result, IsSdCardInserted,                            (ams::sf::Out<bool> out),                                                                                                  (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H,   1, Result, GetSdCardSpeedMode,                          (ams::sf::Out<s64> out),                                                                                                   (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H,   2, Result, GetSdCardCid,                                (ams::sf::OutBuffer out_buffer, s64 size),                                                                                 (out_buffer, size))                                                                           \
    AMS_SF_METHOD_INFO(C, H,   3, Result, GetSdCardUserAreaSize,                       (ams::sf::Out<s64> out),                                                                                                   (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H,   4, Result, GetSdCardProtectedAreaSize,                  (ams::sf::Out<s64> out),                                                                                                   (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H,   5, Result, GetAndClearSdCardErrorInfo,                  (ams::sf::Out<ams::fs::StorageErrorInfo> out_sei, ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buffer, s64 size),    (out_sei, out_size, out_buffer, size))                                                        \
    AMS_SF_METHOD_INFO(C, H,   6, Result, GetSdCardHostControllerStatus,               (ams::sf::Out<ams::fssrv::sf::DeviceOperatorHostControllerStatus> out),                                                    (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H,   7, Result, SetSdCardActivationMode,                     (u8 mode),                                                                                                                 (mode))                                                                                       \
    AMS_SF_METHOD_INFO(C, H,   8, Result, TryGetSdCardInfo,                            (ams::sf::OutBuffer out_buffer, s64 size),                                                                                 (out_buffer, size))                                                                           \
    AMS_SF_METHOD_INFO(C, H, 100, Result, GetMmcCid,                                   (ams::sf::OutBuffer out_buffer, s64 size),                                                                                 (out_buffer, size))                                                                           \
    AMS_SF_METHOD_INFO(C, H, 101, Result, GetMmcSpeedMode,                             (ams::sf::Out<s64> out),                                                                                                   (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H, 110, Result, EraseMmc,                                    (ams::fs::MmcPartition partition),                                                                                         (partition))                                                                                  \
    AMS_SF_METHOD_INFO(C, H, 111, Result, GetMmcPartitionSize,                         (ams::sf::Out<s64> out, ams::fs::MmcPartition partition),                                                                  (out, partition))                                                                             \
    AMS_SF_METHOD_INFO(C, H, 112, Result, GetMmcPatrolCount,                           (ams::sf::Out<u32> out),                                                                                                   (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H, 113, Result, GetAndClearMmcErrorInfo,                     (ams::sf::Out<ams::fs::StorageErrorInfo> out_sei, ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buffer, s64 size),    (out_sei, out_size, out_buffer, size))                                                        \
    AMS_SF_METHOD_INFO(C, H, 114, Result, GetMmcExtendedCsd,                           (ams::sf::OutBuffer out_buffer, s64 size),                                                                                 (out_buffer, size))                                                                           \
    AMS_SF_METHOD_INFO(C, H, 115, Result, SuspendMmcPatrol,                            (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 116, Result, ResumeMmcPatrol,                             (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 117, Result, EraseMmcWithRange,                           (ams::fs::MmcPartition partition, u64 offset, u64 size),                                                                   (partition, offset, size))                                                                    \
    AMS_SF_METHOD_INFO(C, H, 118, Result, MarkBeforeEraseMmcPartitionUserData,         (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 119, Result, CheckAfterEraseMmcPartitionUserData,         (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 200, Result, IsGameCardInserted,                          (ams::sf::Out<bool> out),                                                                                                  (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H, 201, Result, EraseGameCard,                               (u32 game_card_size, u64 normal_area_size),                                                                                (game_card_size, normal_area_size))                                                           \
    AMS_SF_METHOD_INFO(C, H, 202, Result, GetGameCardHandle,                           (ams::sf::Out<u32> out),                                                                                                   (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H, 203, Result, GetGameCardUpdatePartitionInfo,              (ams::sf::Out<ams::fssrv::sf::DeviceOperatorGameCardUpdatePartitionInfo> out, u32 handle),                                 (out, handle))                                                                                \
    AMS_SF_METHOD_INFO(C, H, 204, Result, FinalizeGameCardDriver,                      (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 205, Result, GetGameCardAttribute,                        (ams::sf::Out<ams::fs::GameCardAttribute> out, u32 handle),                                                                (out, handle))                                                                                \
    AMS_SF_METHOD_INFO(C, H, 206, Result, GetGameCardDeviceCertificateDeprecated,      (ams::sf::OutBuffer out_buffer, s64 size, u32 handle),                                                                     (out_buffer, size, handle),    hos::Version_Min, hos::Version_18_1_0)                         \
    AMS_SF_METHOD_INFO(C, H, 206, Result, GetGameCardDeviceCertificate,                (ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buffer, s64 size, u32 handle),                                         (out_size, out_buffer, size, handle),            hos::Version_19_0_0)                         \
    AMS_SF_METHOD_INFO(C, H, 207, Result, GetGameCardAsicInfo,                         (const ams::sf::OutBuffer &rma_info, s64 rma_info_size, const ams::sf::InBuffer &fw_buffer, s64 fw_buffer_size),           (rma_info, rma_info_size, fw_buffer, fw_buffer_size))                                         \
    AMS_SF_METHOD_INFO(C, H, 208, Result, GetGameCardIdSet,                            (ams::sf::OutBuffer out_buffer, s64 size),                                                                                 (out_buffer, size))                                                                           \
    AMS_SF_METHOD_INFO(C, H, 209, Result, WriteToGameCardDirectly,                     (s64 offset, ams::sf::OutBuffer out_buffer, s64 size),                                                                     (offset, out_buffer, size))                                                                   \
    AMS_SF_METHOD_INFO(C, H, 210, Result, SetVerifyWriteEnableFlag,                    (bool en),                                                                                                                 (en))                                                                                         \
    AMS_SF_METHOD_INFO(C, H, 211, Result, GetGameCardImageHash,                        (ams::sf::OutBuffer out_buffer, s64 size, u32 handle),                                                                     (out_buffer, size, handle))                                                                   \
    AMS_SF_METHOD_INFO(C, H, 212, Result, GetGameCardDeviceIdForProdCard,              (ams::sf::OutBuffer out_buffer, s64 size, const ams::sf::InBuffer &card_header, s64 card_header_size),                     (out_buffer, size, card_header, card_header_size))                                            \
    AMS_SF_METHOD_INFO(C, H, 213, Result, EraseAndWriteParamDirectly,                  (const ams::sf::InBuffer &in_buffer, s64 size),                                                                            (in_buffer, size))                                                                            \
    AMS_SF_METHOD_INFO(C, H, 214, Result, ReadParamDirectly,                           (ams::sf::OutBuffer out_buffer, s64 size),                                                                                 (out_buffer, size))                                                                           \
    AMS_SF_METHOD_INFO(C, H, 215, Result, ForceEraseGameCard,                          (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 216, Result, GetGameCardErrorInfo,                        (ams::sf::Out<ams::fssrv::sf::DeviceOperatorGameCardErrorInfo> out),                                                       (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H, 217, Result, GetGameCardErrorReportInfo,                  (ams::sf::Out<ams::fs::GameCardErrorReportInfo> out),                                                                      (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H, 218, Result, GetGameCardDeviceId,                         (ams::sf::OutBuffer out_buffer, s64 size),                                                                                 (out_buffer, size))                                                                           \
    AMS_SF_METHOD_INFO(C, H, 219, Result, ChallengeCardExistence,                      (ams::sf::OutBuffer out_buffer, const ams::sf::InBuffer &seed, const ams::sf::InBuffer &value, u32 handle),                (out_buffer, seed, value, handle))                                                            \
    AMS_SF_METHOD_INFO(C, H, 220, Result, GetGameCardCompatibilityType,                (ams::sf::Out<ams::fs::GameCardCompatibilityType> out, u32 handle),                                                        (out, handle))                                                                                \
    AMS_SF_METHOD_INFO(C, H, 221, Result, GetGameCardAsicCertificate,                  (ams::sf::OutBuffer out_buffer),                                                                                           (out_buffer))                                                                                 \
    AMS_SF_METHOD_INFO(C, H, 222, Result, GetGameCardCardHeader,                       (ams::sf::OutBuffer out_buffer, s64 size, u32 handle),                                                                     (out_buffer, size, handle))                                                                   \
    AMS_SF_METHOD_INFO(C, H, 223, Result, SetGameCardSessionCreationDelay,             (bool enabled, u32 minimum_delay_micro_seconds, bool forced),                                                              (enabled, minimum_delay_micro_seconds, forced))                                               \
    AMS_SF_METHOD_INFO(C, H, 224, Result, GetGameCardApplicationIdList,                (ams::sf::Out<u16> out_count, ams::sf::OutBuffer out_buffer, s64 size, u32 handle),                                        (out_count, out_buffer, size, handle))                                                        \
    AMS_SF_METHOD_INFO(C, H, 225, Result, RegisterGameCardConfigurationData,           (const ams::sf::InBuffer &in_buffer, s64 size),                                                                            (in_buffer, size))                                                                            \
    AMS_SF_METHOD_INFO(C, H, 226, Result, GetGameCardDetailedErrorReportInfo,          (ams::sf::OutBuffer out_buffer),                                                                                           (out_buffer))                                                                                 \
    AMS_SF_METHOD_INFO(C, H, 300, Result, SetSpeedEmulationMode,                       (ams::fs::SpeedEmulationMode mode),                                                                                        (mode))                                                                                       \
    AMS_SF_METHOD_INFO(C, H, 301, Result, GetSpeedEmulationMode,                       (ams::sf::Out<ams::fs::SpeedEmulationMode> out),                                                                           (out))                                                                                        \
    AMS_SF_METHOD_INFO(C, H, 302, Result, SetApplicationStorageSpeed,                  (s32 speed),                                                                                                               (speed))                                                                                      \
    AMS_SF_METHOD_INFO(C, H, 303, Result, SetGameCardClockRateForSpeedEmulation,       (s32 clock_rate),                                                                                                          (clock_rate))                                                                                 \
    AMS_SF_METHOD_INFO(C, H, 304, Result, ClearGameCardClockRateForSpeedEmulation,     (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 400, Result, SuspendSdmmcControl,                         (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 401, Result, ResumeSdmmcControl,                          (),                                                                                                                        ())                                                                                           \
    AMS_SF_METHOD_INFO(C, H, 402, Result, GetSdmmcConnectionStatus,                    (ams::sf::Out<s32> out_a, ams::sf::Out<s32> out_b, u32 port),                                                              (out_a, out_b, port))                                                                         \
    AMS_SF_METHOD_INFO(C, H, 500, Result, SetDeviceSimulationEvent,                    (u32 device_type, u32 detection_mode, u32 access_failure_event, u32 target_operation, u32 access_failure_result),          (device_type, detection_mode, access_failure_event, target_operation, access_failure_result)) \
    AMS_SF_METHOD_INFO(C, H, 501, Result, ClearDeviceSimulationEvent,                  (u32 device_type),                                                                                                         (device_type))     

AMS_SF_DEFINE_MITM_INTERFACE(ams::mitm::fs, IDeviceOperatorMitmInterface, AMS_FS_MITM_I_DEVICE_OPERATOR_INTERFACE_INFO, 0x1484E21C)

namespace ams::mitm::fs {

    class FsRemoteDeviceOperatorMitmService {
        private:
            ::FsDeviceOperator m_operator;

        public:
            explicit FsRemoteDeviceOperatorMitmService(::FsDeviceOperator &o);
            virtual ~FsRemoteDeviceOperatorMitmService();

        public:
            Result IsSdCardInserted(ams::sf::Out<bool> out);
            Result GetSdCardSpeedMode(ams::sf::Out<s64> out);
            Result GetSdCardCid(ams::sf::OutBuffer out_buffer, s64 size);
            Result GetSdCardUserAreaSize(ams::sf::Out<s64> out);
            Result GetSdCardProtectedAreaSize(ams::sf::Out<s64> out);
            Result GetAndClearSdCardErrorInfo(ams::sf::Out<ams::fs::StorageErrorInfo> out_error_info, ams::sf::Out<s64> out_log_size, ams::sf::OutBuffer out_buffer, s64 log_buffer_size);
            Result GetSdCardHostControllerStatus(ams::sf::Out<ams::fssrv::sf::DeviceOperatorHostControllerStatus> out);
            Result SetSdCardActivationMode(u8 mode);
            Result TryGetSdCardInfo(ams::sf::OutBuffer out_buffer, s64 size);

            Result GetMmcCid(ams::sf::OutBuffer out_buffer, s64 size);
            Result GetMmcSpeedMode(ams::sf::Out<s64> out);
            Result EraseMmc(ams::fs::MmcPartition partition);
            Result GetMmcPartitionSize(ams::sf::Out<s64> out, ams::fs::MmcPartition partition);
            Result GetMmcPatrolCount(ams::sf::Out<u32> out);
            Result GetAndClearMmcErrorInfo(ams::sf::Out<ams::fs::StorageErrorInfo> out_error_info, ams::sf::Out<s64> out_log_size, ams::sf::OutBuffer out_buffer, s64 log_buffer_size);
            Result GetMmcExtendedCsd(ams::sf::OutBuffer out_buffer, s64 size);
            Result SuspendMmcPatrol();
            Result ResumeMmcPatrol();
            Result EraseMmcWithRange(ams::fs::MmcPartition partition, u64 offset, u64 size);
            Result MarkBeforeEraseMmcPartitionUserData();
            Result CheckAfterEraseMmcPartitionUserData();

            Result IsGameCardInserted(ams::sf::Out<bool> out);
            Result EraseGameCard(u32 game_card_size, u64 normal_area_size);
            Result GetGameCardHandle(ams::sf::Out<u32> out);
            Result GetGameCardUpdatePartitionInfo(ams::sf::Out<ams::fssrv::sf::DeviceOperatorGameCardUpdatePartitionInfo> out, u32 handle);
            Result FinalizeGameCardDriver();
            Result GetGameCardAttribute(ams::sf::Out<ams::fs::GameCardAttribute> out, u32 handle);
            Result GetGameCardDeviceCertificateDeprecated(ams::sf::OutBuffer out_buffer, s64 size, u32 handle);
            Result GetGameCardDeviceCertificate(ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buffer, s64 size, u32 handle);
            Result GetGameCardAsicInfo(const ams::sf::OutBuffer &rma_info, s64 rma_info_size, const ams::sf::InBuffer &fw_buffer, s64 fw_buffer_size);
            Result GetGameCardIdSet(ams::sf::OutBuffer out_buffer, s64 size);
            Result WriteToGameCardDirectly(s64 offset, ams::sf::OutBuffer out_buffer, s64 size);
            Result SetVerifyWriteEnableFlag(bool en);
            Result GetGameCardImageHash(ams::sf::OutBuffer out_buffer, s64 size, u32 handle);
            Result GetGameCardDeviceIdForProdCard(ams::sf::OutBuffer out_buffer, s64 size, const ams::sf::InBuffer &card_header, s64 card_header_size);
            Result EraseAndWriteParamDirectly(const ams::sf::InBuffer &in_buffer, s64 size);
            Result ReadParamDirectly(ams::sf::OutBuffer out_buffer, s64 size);
            Result ForceEraseGameCard();
            Result GetGameCardErrorInfo(ams::sf::Out<ams::fssrv::sf::DeviceOperatorGameCardErrorInfo> out);
            Result GetGameCardErrorReportInfo(ams::sf::Out<ams::fs::GameCardErrorReportInfo> out);
            Result GetGameCardDeviceId(ams::sf::OutBuffer out_buffer, s64 size);
            Result ChallengeCardExistence(ams::sf::OutBuffer out_buffer, const ams::sf::InBuffer &seed, const ams::sf::InBuffer &value, u32 handle);

            Result GetGameCardCompatibilityType(ams::sf::Out<ams::fs::GameCardCompatibilityType> out, u32 handle);

            Result GetGameCardAsicCertificate(ams::sf::OutBuffer out_buffer);
            Result GetGameCardCardHeader(ams::sf::OutBuffer out_buffer, s64 size, u32 handle);
            Result SetGameCardSessionCreationDelay(bool enabled, u32 minimum_delay_micro_seconds, bool forced);
            Result GetGameCardApplicationIdList(ams::sf::Out<u16> out_count, ams::sf::OutBuffer out_buffer, s64 size, u32 handle);
            Result RegisterGameCardConfigurationData(const ams::sf::InBuffer &in_buffer, s64 size);
            Result GetGameCardDetailedErrorReportInfo(ams::sf::OutBuffer out_buffer);

            Result SetSpeedEmulationMode(ams::fs::SpeedEmulationMode mode);
            Result GetSpeedEmulationMode(ams::sf::Out<ams::fs::SpeedEmulationMode> out);
            Result SetApplicationStorageSpeed(s32 speed);
            Result SetGameCardClockRateForSpeedEmulation(s32 clock_rate);
            Result ClearGameCardClockRateForSpeedEmulation();

            Result SuspendSdmmcControl();
            Result ResumeSdmmcControl();
            Result GetSdmmcConnectionStatus(ams::sf::Out<s32> out_a, ams::sf::Out<s32> out_b, u32 port);

            Result SetDeviceSimulationEvent(u32 device_type, u32 detection_mode, u32 access_failure_event, u32 target_operation, u32 access_failure_result);
            Result ClearDeviceSimulationEvent(u32 device_type);
    };
    static_assert(IsIDeviceOperatorMitmInterface<FsRemoteDeviceOperatorMitmService>);

}
