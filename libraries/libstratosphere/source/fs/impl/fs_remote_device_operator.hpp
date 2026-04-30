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
#include <stratosphere.hpp>

namespace ams::fs::impl {

    #if defined(ATMOSPHERE_OS_HORIZON)
    class RemoteDeviceOperator {
        private:
            ::FsDeviceOperator m_operator;
        public:
            RemoteDeviceOperator(::FsDeviceOperator &o) : m_operator(o) { /* ... */ }

            virtual ~RemoteDeviceOperator() {
                fsDeviceOperatorClose(std::addressof(m_operator));
            }
        public:
            Result IsSdCardInserted(ams::sf::Out<bool> out) {
                R_RETURN(fsDeviceOperatorIsSdCardInserted(std::addressof(m_operator), out.GetPointer()));
            }

            Result GetSdCardSpeedMode(ams::sf::Out<s64> out) {
                R_RETURN(fsDeviceOperatorGetSdCardSpeedMode(std::addressof(m_operator), out.GetPointer()));
            }

            Result GetSdCardCid(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetSdCardCid(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result GetSdCardUserAreaSize(ams::sf::Out<s64> out) {
                R_RETURN(fsDeviceOperatorGetSdCardUserAreaSize(std::addressof(m_operator), out.GetPointer()));
            }

            Result GetSdCardProtectedAreaSize(ams::sf::Out<s64> out) {
                R_RETURN(fsDeviceOperatorGetSdCardProtectedAreaSize(std::addressof(m_operator), out.GetPointer()));
            }

            Result GetAndClearSdCardErrorInfo(ams::sf::Out<fs::StorageErrorInfo> out_sei, ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buf, s64 size) {
                static_assert(sizeof(::FsStorageErrorInfo) == sizeof(fs::StorageErrorInfo));
                R_RETURN(fsDeviceOperatorGetAndClearSdCardErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsStorageErrorInfo *>(out_sei.GetPointer()), out_size.GetPointer(), out_buf.GetPointer(), out_buf.GetSize(), size));
            }

            Result GetSdCardHostControllerStatus(ams::sf::Out<ams::fssrv::sf::DeviceOperatorHostControllerStatus> out) {
                static_assert(sizeof(::FsHostControllerStatus) == sizeof(ams::fssrv::sf::DeviceOperatorHostControllerStatus));
                R_RETURN(fsDeviceOperatorGetSdCardHostControllerStatus(std::addressof(m_operator), reinterpret_cast<::FsHostControllerStatus *>(out.GetPointer())));
            }

            Result SetSdCardActivationMode(u8 mode) {
                R_RETURN(fsDeviceOperatorSetSdCardActivationMode(std::addressof(m_operator), static_cast<FsSdCardActivationMode>(mode)));
            }

            Result TryGetSdCardInfo(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorTryGetSdCardInfo(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result GetMmcCid(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetMmcCid(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result GetMmcSpeedMode(ams::sf::Out<s64> out) {
                R_RETURN(fsDeviceOperatorGetMmcSpeedMode(std::addressof(m_operator), out.GetPointer()));
            }

            Result EraseMmc(ams::fs::MmcPartition partition) {
                R_RETURN(fsDeviceOperatorEraseMmc(std::addressof(m_operator), static_cast<FsMmcPartition>(partition)));
            }

            Result GetMmcPartitionSize(ams::sf::Out<s64> out, ams::fs::MmcPartition partition) {
                R_RETURN(fsDeviceOperatorGetMmcPartitionSize(std::addressof(m_operator), static_cast<FsMmcPartition>(partition), out.GetPointer()));
            }

            Result GetMmcPatrolCount(ams::sf::Out<u32> out) {
                R_RETURN(fsDeviceOperatorGetMmcPatrolCount(std::addressof(m_operator), out.GetPointer()));
            }

            Result GetAndClearMmcErrorInfo(ams::sf::Out<fs::StorageErrorInfo> out_sei, ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buf, s64 size) {
                static_assert(sizeof(::FsStorageErrorInfo) == sizeof(fs::StorageErrorInfo));
                R_RETURN(fsDeviceOperatorGetAndClearMmcErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsStorageErrorInfo *>(out_sei.GetPointer()), out_size.GetPointer(), out_buf.GetPointer(), out_buf.GetSize(), size));
            }

            Result GetMmcExtendedCsd(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetMmcExtendedCsd(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result SuspendMmcPatrol() {
                R_RETURN(fsDeviceOperatorSuspendMmcPatrol(std::addressof(m_operator)));
            }

            Result ResumeMmcPatrol() {
                R_RETURN(fsDeviceOperatorResumeMmcPatrol(std::addressof(m_operator)));
            }

            Result EraseMmcWithRange(ams::fs::MmcPartition partition, u64 offset, u64 size) {
                R_RETURN(fsDeviceOperatorEraseMmcWithRange(std::addressof(m_operator), static_cast<FsMmcPartition>(partition), offset, size));
            }
            Result MarkBeforeEraseMmcPartitionUserData() {
                R_RETURN(fsDeviceOperatorMarkBeforeEraseMmcPartitionUserData(std::addressof(m_operator)));
            }

            Result CheckAfterEraseMmcPartitionUserData() {
                R_RETURN(fsDeviceOperatorCheckAfterEraseMmcPartitionUserData(std::addressof(m_operator)));
            } 

            Result IsGameCardInserted(ams::sf::Out<bool> out) {
                R_RETURN(fsDeviceOperatorIsGameCardInserted(std::addressof(m_operator), out.GetPointer()));
            }

            Result EraseGameCard(u32 game_card_size, u64 normal_area_size) {
                R_RETURN(fsDeviceOperatorEraseGameCard(std::addressof(m_operator), static_cast<FsGameCardSize>(game_card_size), normal_area_size));
            }

            Result GetGameCardHandle(ams::sf::Out<u32> out) {
                static_assert(sizeof(::FsGameCardHandle) == sizeof(u32));
                R_RETURN(fsDeviceOperatorGetGameCardHandle(std::addressof(m_operator), reinterpret_cast<::FsGameCardHandle *>(out.GetPointer())));
            }

            Result GetGameCardUpdatePartitionInfo(ams::sf::Out<ams::fssrv::sf::DeviceOperatorGameCardUpdatePartitionInfo> out, u32 handle) {
                static_assert(sizeof(::FsGameCardUpdatePartitionInfo) == sizeof(ams::fssrv::sf::DeviceOperatorGameCardUpdatePartitionInfo));
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorGetGameCardUpdatePartitionInfo(std::addressof(m_operator), std::addressof(hnd), reinterpret_cast<::FsGameCardUpdatePartitionInfo *>(out.GetPointer())));
            }

            Result FinalizeGameCardDriver() {
                R_RETURN(fsDeviceOperatorFinalizeGameCardDriver(std::addressof(m_operator)));
            }

            Result GetGameCardAttribute(ams::sf::Out<ams::fs::GameCardAttribute> out, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                static_assert(sizeof(ams::fs::GameCardAttribute) == sizeof(u8));
                R_RETURN(fsDeviceOperatorGetGameCardAttribute(std::addressof(m_operator), std::addressof(hnd), reinterpret_cast<u8 *>(out.GetPointer())));
            }

            Result GetGameCardDeviceCertificateDeprecated(ams::sf::OutBuffer out, s64 size, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorGetGameCardDeviceCertificate(std::addressof(m_operator), std::addressof(hnd), out.GetPointer(), out.GetSize(), nullptr, size));
            }

            Result GetGameCardDeviceCertificate(ams::sf::Out<s64> out_size, ams::sf::OutBuffer out, s64 size, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorGetGameCardDeviceCertificate(std::addressof(m_operator), std::addressof(hnd), out.GetPointer(), out.GetSize(), out_size.GetPointer(), size));
            }

            Result GetGameCardAsicInfo(ams::sf::OutBuffer rma_info, s64 rma_info_size, const ams::sf::InBuffer &fw_buffer, s64 fw_buffer_size) {
                R_RETURN(fsDeviceOperatorGetGameCardAsicInfo(std::addressof(m_operator), fw_buffer.GetPointer(), fw_buffer.GetSize(), fw_buffer_size, rma_info.GetPointer(), rma_info.GetSize(), rma_info_size));
            }

            Result GetGameCardIdSet(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetGameCardIdSet(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result WriteToGameCardDirectly(s64 offset, ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorWriteToGameCardDirectly(std::addressof(m_operator), out.GetPointer(), out.GetSize(), offset, size));
            }

            Result SetVerifyWriteEnableFlag(bool en) {
                R_RETURN(fsDeviceOperatorSetVerifyWriteEnableFlag(std::addressof(m_operator), en));
            }

            Result GetGameCardImageHash(ams::sf::OutBuffer out, s64 size, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorGetGameCardImageHash(std::addressof(m_operator), std::addressof(hnd), out.GetPointer(), out.GetSize(), size));
            }

            Result GetGameCardDeviceIdForProdCard(ams::sf::OutBuffer out, s64 size, const ams::sf::InBuffer &card_header, s64 card_header_size) {
                R_RETURN(fsDeviceOperatorGetGameCardDeviceIdForProdCard(std::addressof(m_operator), card_header.GetPointer(), card_header.GetSize(), card_header_size, out.GetPointer(), out.GetSize(), size));
            }

            Result EraseAndWriteParamDirectly(const ams::sf::InBuffer &in, s64 size) {
                R_RETURN(fsDeviceOperatorEraseAndWriteParamDirectly(std::addressof(m_operator), in.GetPointer(), in.GetSize(), size));
            }

            Result ReadParamDirectly(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorReadParamDirectly(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result ForceEraseGameCard() {
                R_RETURN(fsDeviceOperatorForceEraseGameCard(std::addressof(m_operator)));
            }

            Result GetGameCardErrorInfo(ams::sf::Out<ams::fssrv::sf::DeviceOperatorGameCardErrorInfo> out) {
                static_assert(sizeof(::FsGameCardErrorInfo) == sizeof(ams::fssrv::sf::DeviceOperatorGameCardErrorInfo));
                R_RETURN(fsDeviceOperatorGetGameCardErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardErrorInfo *>(out.GetPointer())));
            }

            Result GetGameCardErrorReportInfo(ams::sf::Out<fs::GameCardErrorReportInfo> out) {
                static_assert(sizeof(::FsGameCardErrorReportInfo) == sizeof(fs::GameCardErrorReportInfo));
                R_RETURN(fsDeviceOperatorGetGameCardErrorReportInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardErrorReportInfo *>(out.GetPointer())));
            }

            Result GetGameCardDeviceId(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetGameCardDeviceId(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result ChallengeCardExistence(ams::sf::OutBuffer out, const ams::sf::InBuffer &seed, const ams::sf::InBuffer &value, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorChallengeCardExistence(std::addressof(m_operator), std::addressof(hnd), out.GetPointer(), out.GetSize(), const_cast<u8 *>(seed.GetPointer()), seed.GetSize(), const_cast<u8 *>(value.GetPointer()), value.GetSize()));
            }

            Result GetGameCardCompatibilityType(ams::sf::Out<ams::fs::GameCardCompatibilityType> out, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                static_assert(sizeof(ams::fs::GameCardCompatibilityType) == sizeof(u8));
                R_RETURN(fsDeviceOperatorGetGameCardCompatibilityType(std::addressof(m_operator), std::addressof(hnd), reinterpret_cast<u8 *>(out.GetPointer())));
            }

            Result GetGameCardAsicCertificate(ams::sf::OutBuffer out) {
                R_RETURN(fsDeviceOperatorGetGameCardAsicCertificate(std::addressof(m_operator), out.GetPointer(), out.GetSize(), out.GetSize()));
            }

            Result GetGameCardCardHeader(ams::sf::OutBuffer out, s64 size, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorGetGameCardCardHeader(std::addressof(m_operator), std::addressof(hnd), out.GetPointer(), out.GetSize(), size));
            }

            Result SetGameCardSessionCreationDelay(bool enabled, u32 minimum_delay_micro_seconds, bool forced) {
                R_RETURN(fsDeviceOperatorSetGameCardSessionCreationDelay(std::addressof(m_operator), enabled, forced, minimum_delay_micro_seconds));
            }

            Result GetGameCardApplicationIdList(ams::sf::Out<u16> out_count, ams::sf::OutBuffer out, s64 size, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorGetGameCardApplicationIdList(std::addressof(m_operator), std::addressof(hnd), out.GetPointer(), out.GetSize(), size, out_count.GetPointer()));
            }

            Result RegisterGameCardConfigurationData(const ams::sf::InBuffer &in, s64 size) {
                R_RETURN(fsDeviceOperatorRegisterGameCardConfigurationData(std::addressof(m_operator), in.GetPointer(), in.GetSize(), size));
            }

            Result GetGameCardDetailedErrorReportInfo(ams::sf::OutBuffer out) {
                R_RETURN(fsDeviceOperatorGetGameCardDetailedErrorReportInfo(std::addressof(m_operator), out.GetPointer(), out.GetSize(), out.GetSize()));
            }

            Result SetSpeedEmulationMode(ams::fs::SpeedEmulationMode mode) {
                R_RETURN(fsDeviceOperatorSetSpeedEmulationMode(std::addressof(m_operator), static_cast<FsSpeedEmulationMode>(mode)));
            }

            Result GetSpeedEmulationMode(ams::sf::Out<ams::fs::SpeedEmulationMode> out) {
                static_assert(sizeof(FsSpeedEmulationMode) == sizeof(ams::fs::SpeedEmulationMode));
                R_RETURN(fsDeviceOperatorGetSpeedEmulationMode(std::addressof(m_operator), reinterpret_cast<::FsSpeedEmulationMode *>(out.GetPointer())));
            }

            Result SetApplicationStorageSpeed(s32 speed) {
                R_RETURN(fsDeviceOperatorSetApplicationStorageSpeed(std::addressof(m_operator), speed));
            }

            Result SetGameCardClockRateForSpeedEmulation(s32 clock_rate) {
                R_RETURN(fsDeviceOperatorSetGameCardClockRateForSpeedEmulation(std::addressof(m_operator), clock_rate));
            }

            Result ClearGameCardClockRateForSpeedEmulation() {
                R_RETURN(fsDeviceOperatorClearGameCardClockRateForSpeedEmulation(std::addressof(m_operator)));
            }

            Result SuspendSdmmcControl() {
                R_RETURN(fsDeviceOperatorSuspendSdmmcControl(std::addressof(m_operator)));
            }

            Result ResumeSdmmcControl() {
                R_RETURN(fsDeviceOperatorResumeSdmmcControl(std::addressof(m_operator)));
            }

            Result GetSdmmcConnectionStatus(ams::sf::Out<s32> out_a, ams::sf::Out<s32> out_b, u32 port) {
                R_RETURN(fsDeviceOperatorGetSdmmcConnectionStatus(std::addressof(m_operator), port, out_a.GetPointer(), out_b.GetPointer()));
            }

            Result SetDeviceSimulationEvent(u32 device_type, u32 detection_mode, u32 access_failure_event, u32 target_operation, u32 access_failure_result) {
                static_assert(sizeof(::FsDeviceSimulationEvent) == sizeof(ams::fssrv::sf::DeviceOperatorDeviceSimulationEvent));
                const ams::fssrv::sf::DeviceOperatorDeviceSimulationEvent event = {
                    .device_type = device_type,
                    .detection_mode = detection_mode,
                    .access_failure_event = access_failure_event,
                    .target_operation = target_operation,
                    .access_failure_result = access_failure_result,
                };
                R_RETURN(fsDeviceOperatorSetDeviceSimulationEvent(std::addressof(m_operator), reinterpret_cast<const ::FsDeviceSimulationEvent *>(std::addressof(event))));
            }

            Result ClearDeviceSimulationEvent(u32 device_type) {
                R_RETURN(fsDeviceOperatorClearDeviceSimulationEvent(std::addressof(m_operator), static_cast<FsSimulatingDeviceType>(device_type)));
            }
    };
    static_assert(fssrv::sf::IsIDeviceOperator<RemoteDeviceOperator>);
    #endif

}
