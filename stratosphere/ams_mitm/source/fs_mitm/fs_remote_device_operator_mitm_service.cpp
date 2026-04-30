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
#include <stratosphere.hpp>
#include "fs_remote_device_operator_mitm_service.hpp"

namespace ams::mitm::fs {

    FsRemoteDeviceOperatorMitmService::FsRemoteDeviceOperatorMitmService(::FsDeviceOperator &o) : m_operator(o) { /* ... */ }

    FsRemoteDeviceOperatorMitmService::~FsRemoteDeviceOperatorMitmService() {
        fsDeviceOperatorClose(std::addressof(m_operator));
    }

    Result FsRemoteDeviceOperatorMitmService::IsSdCardInserted(ams::sf::Out<bool> out) {
        R_RETURN(fsDeviceOperatorIsSdCardInserted(std::addressof(m_operator), out.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::GetSdCardSpeedMode(ams::sf::Out<s64> out) {
        R_RETURN(fsDeviceOperatorGetSdCardSpeedMode(std::addressof(m_operator), out.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::GetSdCardCid(ams::sf::OutBuffer out_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorGetSdCardCid(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetSdCardUserAreaSize(ams::sf::Out<s64> out) {
        R_RETURN(fsDeviceOperatorGetSdCardUserAreaSize(std::addressof(m_operator), out.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::GetSdCardProtectedAreaSize(ams::sf::Out<s64> out) {
        R_RETURN(fsDeviceOperatorGetSdCardProtectedAreaSize(std::addressof(m_operator), out.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::GetAndClearSdCardErrorInfo(ams::sf::Out<ams::fs::StorageErrorInfo> out_error_info, ams::sf::Out<s64> out_log_size, ams::sf::OutBuffer out_buffer, s64 log_buffer_size) {
        static_assert(sizeof(::FsStorageErrorInfo) == sizeof(ams::fs::StorageErrorInfo));
        R_RETURN(fsDeviceOperatorGetAndClearSdCardErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsStorageErrorInfo *>(out_error_info.GetPointer()), out_log_size.GetPointer(), out_buffer.GetPointer(), out_buffer.GetSize(), log_buffer_size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetSdCardHostControllerStatus(ams::sf::Out<ams::fssrv::sf::DeviceOperatorHostControllerStatus> out) {
        static_assert(sizeof(::FsHostControllerStatus) == sizeof(ams::fssrv::sf::DeviceOperatorHostControllerStatus));
        R_RETURN(fsDeviceOperatorGetSdCardHostControllerStatus(std::addressof(m_operator), reinterpret_cast<::FsHostControllerStatus *>(out.GetPointer())));
    }

    Result FsRemoteDeviceOperatorMitmService::SetSdCardActivationMode(u8 mode) {
        R_RETURN(fsDeviceOperatorSetSdCardActivationMode(std::addressof(m_operator), static_cast<FsSdCardActivationMode>(mode)));
    }

    Result FsRemoteDeviceOperatorMitmService::TryGetSdCardInfo(ams::sf::OutBuffer out_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorTryGetSdCardInfo(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetMmcCid(ams::sf::OutBuffer out_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorGetMmcCid(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetMmcSpeedMode(ams::sf::Out<s64> out) {
        R_RETURN(fsDeviceOperatorGetMmcSpeedMode(std::addressof(m_operator), out.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::EraseMmc(ams::fs::MmcPartition partition) {
        R_RETURN(fsDeviceOperatorEraseMmc(std::addressof(m_operator), static_cast<FsMmcPartition>(partition)));
    }

    Result FsRemoteDeviceOperatorMitmService::GetMmcPartitionSize(ams::sf::Out<s64> out, ams::fs::MmcPartition partition) {
        R_RETURN(fsDeviceOperatorGetMmcPartitionSize(std::addressof(m_operator), static_cast<FsMmcPartition>(partition), out.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::GetMmcPatrolCount(ams::sf::Out<u32> out) {
        R_RETURN(fsDeviceOperatorGetMmcPatrolCount(std::addressof(m_operator), out.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::GetAndClearMmcErrorInfo(ams::sf::Out<ams::fs::StorageErrorInfo> out_error_info, ams::sf::Out<s64> out_log_size, ams::sf::OutBuffer out_buffer, s64 log_buffer_size) {
        static_assert(sizeof(::FsStorageErrorInfo) == sizeof(ams::fs::StorageErrorInfo));
        R_RETURN(fsDeviceOperatorGetAndClearMmcErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsStorageErrorInfo *>(out_error_info.GetPointer()), out_log_size.GetPointer(), out_buffer.GetPointer(), out_buffer.GetSize(), log_buffer_size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetMmcExtendedCsd(ams::sf::OutBuffer out_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorGetMmcExtendedCsd(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::SuspendMmcPatrol() {
        R_RETURN(fsDeviceOperatorSuspendMmcPatrol(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::ResumeMmcPatrol() {
        R_RETURN(fsDeviceOperatorResumeMmcPatrol(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::EraseMmcWithRange(ams::fs::MmcPartition partition, u64 offset, u64 size) {
        R_RETURN(fsDeviceOperatorEraseMmcWithRange(std::addressof(m_operator), static_cast<FsMmcPartition>(partition), offset, size));
    }

    Result FsRemoteDeviceOperatorMitmService::MarkBeforeEraseMmcPartitionUserData() {
        R_RETURN(fsDeviceOperatorMarkBeforeEraseMmcPartitionUserData(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::CheckAfterEraseMmcPartitionUserData() {
        R_RETURN(fsDeviceOperatorCheckAfterEraseMmcPartitionUserData(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::IsGameCardInserted(ams::sf::Out<bool> out) {
        R_RETURN(fsDeviceOperatorIsGameCardInserted(std::addressof(m_operator), out.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::EraseGameCard(u32 game_card_size, u64 normal_area_size) {
        R_RETURN(fsDeviceOperatorEraseGameCard(std::addressof(m_operator), static_cast<FsGameCardSize>(game_card_size), normal_area_size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardHandle(ams::sf::Out<u32> out) {
        static_assert(sizeof(::FsGameCardHandle) == sizeof(u32));
        R_RETURN(fsDeviceOperatorGetGameCardHandle(std::addressof(m_operator), reinterpret_cast<::FsGameCardHandle *>(out.GetPointer())));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardUpdatePartitionInfo(ams::sf::Out<ams::fssrv::sf::DeviceOperatorGameCardUpdatePartitionInfo> out, u32 handle) {
        static_assert(sizeof(::FsGameCardUpdatePartitionInfo) == sizeof(ams::fssrv::sf::DeviceOperatorGameCardUpdatePartitionInfo));
        const ::FsGameCardHandle hnd = { handle };
        R_RETURN(fsDeviceOperatorGetGameCardUpdatePartitionInfo(std::addressof(m_operator), std::addressof(hnd), reinterpret_cast<::FsGameCardUpdatePartitionInfo *>(out.GetPointer())));
    }

    Result FsRemoteDeviceOperatorMitmService::FinalizeGameCardDriver() {
        R_RETURN(fsDeviceOperatorFinalizeGameCardDriver(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardAttribute(ams::sf::Out<ams::fs::GameCardAttribute> out, u32 handle) {
        const ::FsGameCardHandle hnd = { handle };
        static_assert(sizeof(ams::fs::GameCardAttribute) == sizeof(u8));
        R_RETURN(fsDeviceOperatorGetGameCardAttribute(std::addressof(m_operator), std::addressof(hnd), reinterpret_cast<u8 *>(out.GetPointer())));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardDeviceCertificateDeprecated(ams::sf::OutBuffer out_buffer, s64 size, u32 handle) {
        const ::FsGameCardHandle hnd = { handle };
        R_RETURN(fsDeviceOperatorGetGameCardDeviceCertificate(std::addressof(m_operator), std::addressof(hnd), out_buffer.GetPointer(), out_buffer.GetSize(), nullptr, size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardDeviceCertificate(ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buffer, s64 size, u32 handle) {
        const ::FsGameCardHandle hnd = { handle };
        R_RETURN(fsDeviceOperatorGetGameCardDeviceCertificate(std::addressof(m_operator), std::addressof(hnd), out_buffer.GetPointer(), out_buffer.GetSize(), out_size.GetPointer(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardAsicInfo(const ams::sf::OutBuffer &rma_info, s64 rma_info_size, const ams::sf::InBuffer &fw_buffer, s64 fw_buffer_size) {
        R_RETURN(fsDeviceOperatorGetGameCardAsicInfo(std::addressof(m_operator), fw_buffer.GetPointer(), fw_buffer.GetSize(), fw_buffer_size, rma_info.GetPointer(), rma_info.GetSize(), rma_info_size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardIdSet(ams::sf::OutBuffer out_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorGetGameCardIdSet(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::WriteToGameCardDirectly(s64 offset, ams::sf::OutBuffer out_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorWriteToGameCardDirectly(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), offset, size));
    }

    Result FsRemoteDeviceOperatorMitmService::SetVerifyWriteEnableFlag(bool en) {
        R_RETURN(fsDeviceOperatorSetVerifyWriteEnableFlag(std::addressof(m_operator), en));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardImageHash(ams::sf::OutBuffer out_buffer, s64 size, u32 handle) {
        const ::FsGameCardHandle hnd = { handle };
        R_RETURN(fsDeviceOperatorGetGameCardImageHash(std::addressof(m_operator), std::addressof(hnd), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardDeviceIdForProdCard(ams::sf::OutBuffer out_buffer, s64 size, const ams::sf::InBuffer &card_header, s64 card_header_size) {
        R_RETURN(fsDeviceOperatorGetGameCardDeviceIdForProdCard(std::addressof(m_operator), card_header.GetPointer(), card_header.GetSize(), card_header_size, out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::EraseAndWriteParamDirectly(const ams::sf::InBuffer &in_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorEraseAndWriteParamDirectly(std::addressof(m_operator), in_buffer.GetPointer(), in_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::ReadParamDirectly(ams::sf::OutBuffer out_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorReadParamDirectly(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::ForceEraseGameCard() {
        R_RETURN(fsDeviceOperatorForceEraseGameCard(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardErrorInfo(ams::sf::Out<ams::fssrv::sf::DeviceOperatorGameCardErrorInfo> out) {
        static_assert(sizeof(::FsGameCardErrorInfo) == sizeof(ams::fssrv::sf::DeviceOperatorGameCardErrorInfo));
        R_RETURN(fsDeviceOperatorGetGameCardErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardErrorInfo *>(out.GetPointer())));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardErrorReportInfo(ams::sf::Out<ams::fs::GameCardErrorReportInfo> out) {
        static_assert(sizeof(::FsGameCardErrorReportInfo) == sizeof(ams::fs::GameCardErrorReportInfo));
        R_RETURN(fsDeviceOperatorGetGameCardErrorReportInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardErrorReportInfo *>(out.GetPointer())));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardDeviceId(ams::sf::OutBuffer out_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorGetGameCardDeviceId(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::ChallengeCardExistence(ams::sf::OutBuffer out_buffer, const ams::sf::InBuffer &seed, const ams::sf::InBuffer &value, u32 handle) {
        const ::FsGameCardHandle hnd = { handle };
        R_RETURN(fsDeviceOperatorChallengeCardExistence(std::addressof(m_operator), std::addressof(hnd), out_buffer.GetPointer(), out_buffer.GetSize(), const_cast<u8 *>(seed.GetPointer()), seed.GetSize(), const_cast<u8 *>(value.GetPointer()), value.GetSize()));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardCompatibilityType(ams::sf::Out<ams::fs::GameCardCompatibilityType> out, u32 handle) {
        AMS_UNUSED(handle);

        out.SetValue(ams::fs::GameCardCompatibilityType::Normal);
        R_SUCCEED();
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardAsicCertificate(ams::sf::OutBuffer out_buffer) {
        R_RETURN(fsDeviceOperatorGetGameCardAsicCertificate(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), out_buffer.GetSize()));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardCardHeader(ams::sf::OutBuffer out_buffer, s64 size, u32 handle) {
        const ::FsGameCardHandle hnd = { handle };
        R_RETURN(fsDeviceOperatorGetGameCardCardHeader(std::addressof(m_operator), std::addressof(hnd), out_buffer.GetPointer(), out_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::SetGameCardSessionCreationDelay(bool enabled, u32 minimum_delay_micro_seconds, bool forced) {
        R_RETURN(fsDeviceOperatorSetGameCardSessionCreationDelay(std::addressof(m_operator), enabled, forced, minimum_delay_micro_seconds));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardApplicationIdList(ams::sf::Out<u16> out_count, ams::sf::OutBuffer out_buffer, s64 size, u32 handle) {
        const ::FsGameCardHandle hnd = { handle };
        R_RETURN(fsDeviceOperatorGetGameCardApplicationIdList(std::addressof(m_operator), std::addressof(hnd), out_buffer.GetPointer(), out_buffer.GetSize(), size, out_count.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::RegisterGameCardConfigurationData(const ams::sf::InBuffer &in_buffer, s64 size) {
        R_RETURN(fsDeviceOperatorRegisterGameCardConfigurationData(std::addressof(m_operator), in_buffer.GetPointer(), in_buffer.GetSize(), size));
    }

    Result FsRemoteDeviceOperatorMitmService::GetGameCardDetailedErrorReportInfo(ams::sf::OutBuffer out_buffer) {
        R_RETURN(fsDeviceOperatorGetGameCardDetailedErrorReportInfo(std::addressof(m_operator), out_buffer.GetPointer(), out_buffer.GetSize(), out_buffer.GetSize()));
    }

    Result FsRemoteDeviceOperatorMitmService::SetSpeedEmulationMode(ams::fs::SpeedEmulationMode mode) {
        R_RETURN(fsDeviceOperatorSetSpeedEmulationMode(std::addressof(m_operator), static_cast<FsSpeedEmulationMode>(mode)));
    }

    Result FsRemoteDeviceOperatorMitmService::GetSpeedEmulationMode(ams::sf::Out<ams::fs::SpeedEmulationMode> out) {
        static_assert(sizeof(FsSpeedEmulationMode) == sizeof(ams::fs::SpeedEmulationMode));
        R_RETURN(fsDeviceOperatorGetSpeedEmulationMode(std::addressof(m_operator), reinterpret_cast<::FsSpeedEmulationMode *>(out.GetPointer())));
    }

    Result FsRemoteDeviceOperatorMitmService::SetApplicationStorageSpeed(s32 speed) {
        R_RETURN(fsDeviceOperatorSetApplicationStorageSpeed(std::addressof(m_operator), speed));
    }

    Result FsRemoteDeviceOperatorMitmService::SetGameCardClockRateForSpeedEmulation(s32 clock_rate) {
        R_RETURN(fsDeviceOperatorSetGameCardClockRateForSpeedEmulation(std::addressof(m_operator), clock_rate));
    }

    Result FsRemoteDeviceOperatorMitmService::ClearGameCardClockRateForSpeedEmulation() {
        R_RETURN(fsDeviceOperatorClearGameCardClockRateForSpeedEmulation(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::SuspendSdmmcControl() {
        R_RETURN(fsDeviceOperatorSuspendSdmmcControl(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::ResumeSdmmcControl() {
        R_RETURN(fsDeviceOperatorResumeSdmmcControl(std::addressof(m_operator)));
    }

    Result FsRemoteDeviceOperatorMitmService::GetSdmmcConnectionStatus(ams::sf::Out<s32> out_a, ams::sf::Out<s32> out_b, u32 port) {
        R_RETURN(fsDeviceOperatorGetSdmmcConnectionStatus(std::addressof(m_operator), port, out_a.GetPointer(), out_b.GetPointer()));
    }

    Result FsRemoteDeviceOperatorMitmService::SetDeviceSimulationEvent(u32 device_type, u32 detection_mode, u32 access_failure_event, u32 target_operation, u32 access_failure_result) {
        const ams::fssrv::sf::DeviceOperatorDeviceSimulationEvent event = {
            .device_type           = device_type,
            .detection_mode        = detection_mode,
            .access_failure_event  = access_failure_event,
            .target_operation      = target_operation,
            .access_failure_result = access_failure_result,
        };
        static_assert(sizeof(::FsDeviceSimulationEvent) == sizeof(event));
        R_RETURN(fsDeviceOperatorSetDeviceSimulationEvent(std::addressof(m_operator), reinterpret_cast<const ::FsDeviceSimulationEvent *>(std::addressof(event))));
    }

    Result FsRemoteDeviceOperatorMitmService::ClearDeviceSimulationEvent(u32 device_type) {
        R_RETURN(fsDeviceOperatorClearDeviceSimulationEvent(std::addressof(m_operator), static_cast<FsSimulatingDeviceType>(device_type)));
    }

}
