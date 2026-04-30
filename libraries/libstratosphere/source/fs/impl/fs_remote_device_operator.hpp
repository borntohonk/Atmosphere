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
                s64 speed_mode;
                R_TRY(fsDeviceOperatorGetSdCardSpeedMode(std::addressof(m_operator), std::addressof(speed_mode)));

                out.SetValue(speed_mode);
                R_SUCCEED();
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

            Result GetSdCardHostControllerStatus(ams::sf::Out<ams::fs::SdCardHostControllerStatus> out) {
                static_assert(sizeof(::FsSdCardHostControllerStatus) == sizeof(ams::fs::SdCardHostControllerStatus));
                R_RETURN(fsDeviceOperatorGetSdCardHostControllerStatus(std::addressof(m_operator), reinterpret_cast<::FsSdCardHostControllerStatus *>(out.GetPointer())));
            }

            Result SetSdCardActivationMode(u8 mode) {
                R_RETURN(fsDeviceOperatorSetSdCardActivationMode(std::addressof(m_operator), mode));
            }

            Result TryGetSdCardInfo(ams::sf::OutBuffer out, s64 size) {
                static_assert(sizeof(::FsSdCardInfo) == sizeof(ams::fssrv::sf::DeviceOperatorSdCardInfo));
                R_UNLESS(size == static_cast<s64>(sizeof(ams::fssrv::sf::DeviceOperatorSdCardInfo)), fs::ResultInvalidSize());
                R_UNLESS(out.GetSize() >= sizeof(ams::fssrv::sf::DeviceOperatorSdCardInfo), fs::ResultInvalidSize());

                ::FsSdCardInfo info;
                R_TRY(fsDeviceOperatorTryGetSdCardInfo(std::addressof(m_operator), std::addressof(info), sizeof(info)));

                std::memcpy(out.GetPointer(), std::addressof(info), sizeof(info));
                R_SUCCEED();
            }

            Result GetMmcCid(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetMmcCid(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result GetMmcSpeedMode(ams::sf::Out<s64> out) {
                s64 speed_mode;
                R_TRY(fsDeviceOperatorGetMmcSpeedMode(std::addressof(m_operator), std::addressof(speed_mode)));

                out.SetValue(speed_mode);
                R_SUCCEED();
            }

            Result EraseMmc(u32 partition) {
                R_RETURN(fsDeviceOperatorEraseMmc(std::addressof(m_operator), partition));
            }

            Result GetMmcPartitionSize(ams::sf::Out<u64> out, u32 partition) {
                R_RETURN(fsDeviceOperatorGetMmcPartitionSize(std::addressof(m_operator), out.GetPointer(), partition));
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

            Result EraseMmcWithRange(u64 offset, u64 size, u64 end) {
                R_RETURN(fsDeviceOperatorEraseMmcWithRange(std::addressof(m_operator), offset, size, end));
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
                R_RETURN(fsDeviceOperatorEraseGameCard(std::addressof(m_operator), game_card_size, normal_area_size));
            }

            Result GetGameCardHandle(ams::sf::Out<u32> out) {
                static_assert(sizeof(::FsGameCardHandle) == sizeof(u32));
                R_RETURN(fsDeviceOperatorGetGameCardHandle(std::addressof(m_operator), reinterpret_cast<::FsGameCardHandle *>(out.GetPointer())));
            }

            Result GetGameCardUpdatePartitionInfo(ams::sf::Out<u32> out_version, ams::sf::Out<u64> out_id, u32 handle) {
                R_RETURN(fsDeviceOperatorGetGameCardUpdatePartitionInfo(std::addressof(m_operator), out_version.GetPointer(), out_id.GetPointer(), handle));
            }

            Result FinalizeGameCardDriver() {
                R_RETURN(fsDeviceOperatorFinalizeGameCardDriver(std::addressof(m_operator)));
            }

            Result GetGameCardAttribute(ams::sf::Out<u8> out, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorGetGameCardAttribute(std::addressof(m_operator), std::addressof(hnd), out.GetPointer()));
            }

            Result GetGameCardDeviceCertificate(ams::sf::Out<s64> out_size, ams::sf::OutBuffer out, s64 buf_size, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorGetGameCardDeviceCertificate(std::addressof(m_operator), std::addressof(hnd), out.GetPointer(), out.GetSize(), out_size.GetPointer(), buf_size));
            }

            Result GetGameCardAsicInfo(ams::sf::OutBuffer rma_info, s64 rma_info_size, ams::sf::InBuffer fw_buffer, s64 fw_buffer_size) {
                static_assert(sizeof(::FsRmaInformation) == sizeof(ams::fssrv::sf::DeviceOperatorGameCardRmaInformation));
                R_UNLESS(rma_info_size == static_cast<s64>(sizeof(ams::fssrv::sf::DeviceOperatorGameCardRmaInformation)), fs::ResultInvalidSize());
                R_UNLESS(rma_info.GetSize() >= sizeof(ams::fssrv::sf::DeviceOperatorGameCardRmaInformation), fs::ResultInvalidSize());
                R_UNLESS(fw_buffer_size == static_cast<s64>(fw_buffer.GetSize()), fs::ResultInvalidSize());

                ::FsRmaInformation info;
                R_TRY(fsDeviceOperatorGetGameCardAsicInfo(std::addressof(m_operator), std::addressof(info), sizeof(info), fw_buffer.GetPointer(), fw_buffer.GetSize()));

                std::memcpy(rma_info.GetPointer(), std::addressof(info), sizeof(info));
                R_SUCCEED();
            }

            Result GetGameCardIdSet(ams::sf::OutBuffer out, s64 size) {
                R_UNLESS(size == static_cast<s64>(sizeof(ams::fssrv::sf::DeviceOperatorGameCardIdSet)), fs::ResultInvalidSize());
                R_UNLESS(out.GetSize() >= sizeof(ams::fssrv::sf::DeviceOperatorGameCardIdSet), fs::ResultInvalidSize());

                ams::fssrv::sf::DeviceOperatorGameCardIdSet id_set;
                R_TRY(fsDeviceOperatorGetGameCardIdSet(std::addressof(m_operator), std::addressof(id_set), sizeof(id_set), size));

                std::memcpy(out.GetPointer(), std::addressof(id_set), sizeof(id_set));
                R_SUCCEED();
            }

            Result WriteToGameCardDirectly(u64 offset, ams::sf::OutBuffer buf) {
                R_RETURN(fsDeviceOperatorWriteToGameCardDirectly(std::addressof(m_operator), offset, buf.GetPointer(), buf.GetSize()));
            }

            Result SetVerifyWriteEnableFlag(bool flag) {
                R_RETURN(fsDeviceOperatorSetVerifyWriteEnableFlag(std::addressof(m_operator), flag));
            }

            Result GetGameCardImageHash(ams::sf::OutBuffer out_hash, u32 handle) {
                R_RETURN(fsDeviceOperatorGetGameCardImageHash(std::addressof(m_operator), out_hash.GetPointer(), out_hash.GetSize(), handle));
            }

            Result GetGameCardDeviceIdForProdCard(ams::sf::OutBuffer out, s64 size, ams::sf::InBuffer card_header, s64 card_header_size) {
                R_UNLESS(size == static_cast<s64>(out.GetSize()), fs::ResultInvalidSize());
                R_UNLESS(card_header_size == static_cast<s64>(card_header.GetSize()), fs::ResultInvalidSize());

                R_RETURN(fsDeviceOperatorGetGameCardDeviceIdForProdCard(std::addressof(m_operator), out.GetPointer(), out.GetSize(), card_header.GetPointer(), card_header.GetSize()));
            }

            Result EraseAndWriteParamDirectly(ams::sf::InBuffer in, s64 size) {
                R_UNLESS(size == static_cast<s64>(in.GetSize()), fs::ResultInvalidSize());
                R_RETURN(fsDeviceOperatorEraseAndWriteParamDirectly(std::addressof(m_operator), in.GetPointer(), in.GetSize()));
            }

            Result ReadParamDirectly(ams::sf::OutBuffer out, s64 size) {
                R_UNLESS(size == static_cast<s64>(out.GetSize()), fs::ResultInvalidSize());
                R_RETURN(fsDeviceOperatorReadParamDirectly(std::addressof(m_operator), out.GetPointer(), out.GetSize()));
            }

            Result ForceEraseGameCard() {
                R_RETURN(fsDeviceOperatorForceEraseGameCard(std::addressof(m_operator)));
            }

            Result GetGameCardErrorInfo(ams::sf::Out<ams::fs::GameCardErrorInfo> out) {
                static_assert(sizeof(::FsGameCardErrorInfo) == sizeof(ams::fs::GameCardErrorInfo));
                R_RETURN(fsDeviceOperatorGetGameCardErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardErrorInfo *>(out.GetPointer())));
            }

            Result GetGameCardErrorReportInfo(ams::sf::Out<fs::GameCardErrorReportInfo> out) {
                static_assert(sizeof(::FsGameCardErrorReportInfo) == sizeof(fs::GameCardErrorReportInfo));
                R_RETURN(fsDeviceOperatorGetGameCardErrorReportInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardErrorReportInfo *>(out.GetPointer())));
            }

            Result GetGameCardDeviceId(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetGameCardDeviceId(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            Result ChallengeCardExistence(ams::sf::OutBuffer out, ams::sf::InBuffer seed, ams::sf::InBuffer value, u32 handle) {
                const ::FsGameCardHandle hnd = { handle };
                R_RETURN(fsDeviceOperatorChallengeCardExistence(std::addressof(m_operator), std::addressof(hnd), out.GetPointer(), out.GetSize(), const_cast<u8 *>(seed.GetPointer()), seed.GetSize(), const_cast<u8 *>(value.GetPointer()), value.GetSize()));
            }

            Result GetGameCardCompatibilityType(ams::sf::Out<u8> out, u32 handle) {
                R_RETURN(fsDeviceOperatorGetGameCardCompatibilityType(std::addressof(m_operator), handle, out.GetPointer()));
            }

            Result GetGameCardAsicCertificate(ams::sf::OutBuffer out, s64 size) {
                static_assert(sizeof(::FsGameCardAsicCertificateSet) == sizeof(ams::fssrv::sf::DeviceOperatorGameCardAsicCertificateSet));
                R_UNLESS(size == static_cast<s64>(sizeof(ams::fssrv::sf::DeviceOperatorGameCardAsicCertificateSet)), fs::ResultInvalidSize());
                R_UNLESS(out.GetSize() >= sizeof(ams::fssrv::sf::DeviceOperatorGameCardAsicCertificateSet), fs::ResultInvalidSize());

                ::FsGameCardAsicCertificateSet certificate;
                R_TRY(fsDeviceOperatorGetGameCardAsicCertificate(std::addressof(m_operator), std::addressof(certificate), sizeof(certificate)));

                std::memcpy(out.GetPointer(), std::addressof(certificate), sizeof(certificate));
                R_SUCCEED();
            }

            Result GetGameCardCardHeaderSet(ams::sf::OutBuffer out_buf, s64 out_size, u32 handle) {
                R_UNLESS(out_size == static_cast<s64>(out_buf.GetSize()), fs::ResultInvalidSize());
                R_RETURN(fsDeviceOperatorGetGameCardCardHeader(std::addressof(m_operator), out_buf.GetPointer(), out_buf.GetSize(), handle));
            }

            Result SetGameCardSessionCreationDelay(bool enabled, u32 delay, bool flag) {
                R_RETURN(fsDeviceOperatorSetGameCardSessionCreationDelay(std::addressof(m_operator), enabled, delay, flag));
            }

            Result GetGameCardApplicationIdList(ams::sf::Out<u16> out_count, ams::sf::OutBuffer out, s64 out_size, u32 handle) {
                R_UNLESS(out_size == static_cast<s64>(out.GetSize()), fs::ResultInvalidSize());
                R_RETURN(fsDeviceOperatorGetGameCardApplicationIdList(std::addressof(m_operator), out_count.GetPointer(), out.GetPointer(), out.GetSize(), handle));
            }

            Result RegisterGameCardConfigurationData(ams::sf::InBuffer in, s64 in_size) {
                R_UNLESS(in_size == static_cast<s64>(in.GetSize()), fs::ResultInvalidSize());
                R_RETURN(fsDeviceOperatorRegisterGameCardConfigurationData(std::addressof(m_operator), in.GetPointer(), in.GetSize()));
            }

            Result GetGameCardDetailedErrorReportInfo(ams::sf::OutBuffer out, s64 size) {
                static_assert(sizeof(::FsGameCardDetailedErrorReportInfo) == sizeof(ams::fssrv::sf::DeviceOperatorGameCardDetailedErrorReportInfo));
                R_UNLESS(size == static_cast<s64>(sizeof(ams::fssrv::sf::DeviceOperatorGameCardDetailedErrorReportInfo)), fs::ResultInvalidSize());
                R_UNLESS(out.GetSize() >= sizeof(ams::fssrv::sf::DeviceOperatorGameCardDetailedErrorReportInfo), fs::ResultInvalidSize());

                ::FsGameCardDetailedErrorReportInfo info;
                R_TRY(fsDeviceOperatorGetGameCardDetailedErrorReportInfo(std::addressof(m_operator), std::addressof(info), sizeof(info)));

                std::memcpy(out.GetPointer(), std::addressof(info), sizeof(info));
                R_SUCCEED();
            }

            Result SetSpeedEmulationMode(u32 mode) {
                R_RETURN(fsDeviceOperatorSetSpeedEmulationMode(std::addressof(m_operator), mode));
            }

            Result GetSpeedEmulationMode(ams::sf::Out<u32> out) {
                R_RETURN(fsDeviceOperatorGetSpeedEmulationMode(std::addressof(m_operator), out.GetPointer()));
            }

            Result SetApplicationStorageSpeed(u32 speed) {
                R_RETURN(fsDeviceOperatorSetApplicationStorageSpeed(std::addressof(m_operator), speed));
            }

            Result SetGameCardClockRateForSpeedEmulation(u32 clock_rate) {
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

            Result GetSdmmcConnectionStatus(ams::sf::Out<u32> out_speed_mode, ams::sf::Out<u32> out_bus_width, u32 port) {
                R_RETURN(fsDeviceOperatorGetSdmmcConnectionStatus(std::addressof(m_operator), out_speed_mode.GetPointer(), out_bus_width.GetPointer(), port));
            }

            Result SetDeviceSimulationEvent(u32 device_type, u32 target_op, u32 failure_type, u32 result_value, u8 flag) {
                R_RETURN(fsDeviceOperatorSetDeviceSimulationEvent(std::addressof(m_operator), device_type, target_op, failure_type, result_value, flag));
            }

            Result ClearDeviceSimulationEvent(u32 device_type) {
                R_RETURN(fsDeviceOperatorClearDeviceSimulationEvent(std::addressof(m_operator), device_type));
            }
    };
    static_assert(fssrv::sf::IsIDeviceOperator<RemoteDeviceOperator>);
    #endif

}
