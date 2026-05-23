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
            /* cmd 0 */
            Result IsSdCardInserted(ams::sf::Out<bool> out) {
                R_RETURN(fsDeviceOperatorIsSdCardInserted(std::addressof(m_operator), out.GetPointer()));
            }

            /* cmd 1 */
            Result GetSdCardSpeedMode(ams::sf::Out<s64> out) {
                R_RETURN(fsDeviceOperatorGetSdCardSpeedMode(std::addressof(m_operator), out.GetPointer()));
            }

            /* cmd 2 */
            Result GetSdCardCid(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetSdCardCid(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            /* cmd 3 */
            Result GetSdCardUserAreaSize(ams::sf::Out<s64> out) {
                R_RETURN(fsDeviceOperatorGetSdCardUserAreaSize(std::addressof(m_operator), out.GetPointer()));
            }

            /* cmd 4 */
            Result GetSdCardProtectedAreaSize(ams::sf::Out<s64> out) {
                R_RETURN(fsDeviceOperatorGetSdCardProtectedAreaSize(std::addressof(m_operator), out.GetPointer()));
            }

            /* cmd 5 */
            Result GetAndClearSdCardErrorInfo(ams::sf::Out<fs::StorageErrorInfo> out_sei, ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buf, s64 size) {
                static_assert(sizeof(::FsStorageErrorInfo) == sizeof(fs::StorageErrorInfo));
                R_RETURN(fsDeviceOperatorGetAndClearSdCardErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsStorageErrorInfo *>(out_sei.GetPointer()), out_size.GetPointer(), out_buf.GetPointer(), out_buf.GetSize(), size));
            }

            /* cmd 6 */
            Result GetSdCardHostControllerStatus(ams::sf::Out<fs::SdCardHostControllerStatus> out) {
                static_assert(sizeof(::FsSdCardHostControllerStatus) == sizeof(fs::SdCardHostControllerStatus));
                R_RETURN(fsDeviceOperatorGetSdCardHostControllerStatus(std::addressof(m_operator), reinterpret_cast<::FsSdCardHostControllerStatus *>(out.GetPointer())));
            }

            /* cmd 7 */
            Result SetSdCardActivationMode(u8 mode) {
                R_RETURN(fsDeviceOperatorSetSdCardActivationMode(std::addressof(m_operator), mode));
            }

            /* cmd 8 */
            Result TryGetSdCardInfo(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorTryGetSdCardInfo(std::addressof(m_operator), reinterpret_cast<::FsSdCardInfo *>(out.GetPointer()), static_cast<u64>(size)));
            }

            /* cmd 100 */
            Result GetMmcCid(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetMmcCid(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            /* cmd 101 */
            Result GetMmcSpeedMode(ams::sf::Out<s64> out) {
                R_RETURN(fsDeviceOperatorGetMmcSpeedMode(std::addressof(m_operator), out.GetPointer()));
            }

            /* cmd 110 */
            Result EraseMmc(u32 partition) {
                R_RETURN(fsDeviceOperatorEraseMmc(std::addressof(m_operator), partition));
            }

            /* cmd 111 */
            Result GetMmcPartitionSize(ams::sf::Out<u64> out, u32 partition) {
                R_RETURN(fsDeviceOperatorGetMmcPartitionSize(std::addressof(m_operator), out.GetPointer(), partition));
            }

            /* cmd 112 */
            Result GetMmcPatrolCount(ams::sf::Out<u32> out) {
                R_RETURN(fsDeviceOperatorGetMmcPatrolCount(std::addressof(m_operator), out.GetPointer()));
            }

            /* cmd 113 */
            Result GetAndClearMmcErrorInfo(ams::sf::Out<fs::StorageErrorInfo> out_sei, ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buf, s64 size) {
                static_assert(sizeof(::FsStorageErrorInfo) == sizeof(fs::StorageErrorInfo));
                R_RETURN(fsDeviceOperatorGetAndClearMmcErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsStorageErrorInfo *>(out_sei.GetPointer()), out_size.GetPointer(), out_buf.GetPointer(), out_buf.GetSize(), size));
            }

            /* cmd 114 */
            Result GetMmcExtendedCsd(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetMmcExtendedCsd(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            /* cmd 115 */
            Result SuspendMmcPatrol() {
                R_RETURN(fsDeviceOperatorSuspendMmcPatrol(std::addressof(m_operator)));
            }

            /* cmd 116 */
            Result ResumeMmcPatrol() {
                R_RETURN(fsDeviceOperatorResumeMmcPatrol(std::addressof(m_operator)));
            }

            /* cmd 117 */
            Result EraseMmcWithRange(u64 offset, u64 size, u64 end) {
                R_RETURN(fsDeviceOperatorEraseMmcWithRange(std::addressof(m_operator), offset, size, end));
            }

            /* cmd 118 */
            Result MarkBeforeEraseMmcPartitionUserData() {
                R_RETURN(fsDeviceOperatorMarkBeforeEraseMmcPartitionUserData(std::addressof(m_operator)));
            }

            /* cmd 119 */
            Result CheckAfterEraseMmcPartitionUserData() {
                R_RETURN(fsDeviceOperatorCheckAfterEraseMmcPartitionUserData(std::addressof(m_operator)));
            }

            /* cmd 200 */
            Result IsGameCardInserted(ams::sf::Out<bool> out) {
                R_RETURN(fsDeviceOperatorIsGameCardInserted(std::addressof(m_operator), out.GetPointer()));
            }

            /* cmd 201 */
            Result EraseGameCard(u32 gc_size, u64 gc_handle) {
                R_RETURN(fsDeviceOperatorEraseGameCard(std::addressof(m_operator), gc_size, gc_handle));
            }

            /* cmd 202 */
            Result GetGameCardHandle(ams::sf::Out<u32> out) {
                static_assert(sizeof(::FsGameCardHandle) == sizeof(u32));
                R_RETURN(fsDeviceOperatorGetGameCardHandle(std::addressof(m_operator), reinterpret_cast<::FsGameCardHandle *>(out.GetPointer())));
            }

            /* cmd 203 */
            Result GetGameCardUpdatePartitionInfo(ams::sf::Out<u32> out_version, ams::sf::Out<u64> out_id, u32 handle) {
                R_RETURN(fsDeviceOperatorGetGameCardUpdatePartitionInfo(std::addressof(m_operator), out_version.GetPointer(), out_id.GetPointer(), handle));
            }

            /* cmd 204 */
            Result FinalizeGameCardDriver() {
                R_RETURN(fsDeviceOperatorFinalizeGameCardDriver(std::addressof(m_operator)));
            }

            /* cmd 205 */
            Result GetGameCardAttribute(ams::sf::Out<u8> out, u32 handle) {
                static_assert(sizeof(::FsGameCardHandle) == sizeof(u32));
                R_RETURN(fsDeviceOperatorGetGameCardAttribute(std::addressof(m_operator), reinterpret_cast<const ::FsGameCardHandle *>(std::addressof(handle)), out.GetPointer()));
            }

            /* cmd 206 */
            Result GetGameCardDeviceCertificate(ams::sf::Out<s64> out_size, ams::sf::OutBuffer out_buf, s64 buf_size, u32 handle) {
                static_assert(sizeof(::FsGameCardHandle) == sizeof(u32));
                R_RETURN(fsDeviceOperatorGetGameCardDeviceCertificate(std::addressof(m_operator), reinterpret_cast<const ::FsGameCardHandle *>(std::addressof(handle)), out_buf.GetPointer(), out_buf.GetSize(), out_size.GetPointer(), buf_size));
            }

            /* cmd 207 */
            Result GetGameCardAsicInfo(ams::sf::OutBuffer out, s64 out_size, ams::sf::InBuffer in_buf, s64 in_size) {
                R_RETURN(fsDeviceOperatorGetGameCardAsicInfo(std::addressof(m_operator), reinterpret_cast<::FsRmaInformation *>(out.GetPointer()), static_cast<u64>(out_size), in_buf.GetPointer(), static_cast<u64>(in_size)));
            }

            /* cmd 208 */
            Result GetGameCardIdSet(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetGameCardIdSet(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            /* cmd 209 */
            Result WriteToGameCardDirectly(u64 offset, ams::sf::OutBuffer buf) {
                R_RETURN(fsDeviceOperatorWriteToGameCardDirectly(std::addressof(m_operator), offset, buf.GetPointer(), buf.GetSize()));
            }

            /* cmd 210 */
            Result SetVerifyWriteEnableFlag(bool flag) {
                R_RETURN(fsDeviceOperatorSetVerifyWriteEnableFlag(std::addressof(m_operator), flag));
            }

            /* cmd 211 */
            Result GetGameCardImageHash(ams::sf::OutBuffer out_hash, u32 handle) {
                R_RETURN(fsDeviceOperatorGetGameCardImageHash(std::addressof(m_operator), out_hash.GetPointer(), out_hash.GetSize(), handle));
            }

            /* cmd 212 */
            Result GetGameCardDeviceIdForProdCard(ams::sf::OutBuffer out_buf, s64 out_size, ams::sf::InBuffer in_buf, s64 in_size) {
                R_RETURN(fsDeviceOperatorGetGameCardDeviceIdForProdCard(std::addressof(m_operator), out_buf.GetPointer(), static_cast<u64>(out_size), in_buf.GetPointer(), static_cast<u64>(in_size)));
            }

            /* cmd 213 */
            Result EraseAndWriteParamDirectly(ams::sf::InBuffer in_buf, s64 in_size) {
                R_RETURN(fsDeviceOperatorEraseAndWriteParamDirectly(std::addressof(m_operator), in_buf.GetPointer(), static_cast<u64>(in_size)));
            }

            /* cmd 214 */
            Result ReadParamDirectly(ams::sf::OutBuffer out_buf, s64 out_size) {
                R_RETURN(fsDeviceOperatorReadParamDirectly(std::addressof(m_operator), out_buf.GetPointer(), static_cast<u64>(out_size)));
            }

            /* cmd 215 */
            Result ForceEraseGameCard() {
                R_RETURN(fsDeviceOperatorForceEraseGameCard(std::addressof(m_operator)));
            }

            /* cmd 216 */
            Result GetGameCardErrorInfo(ams::sf::Out<ams::fs::GameCardErrorInfo> out) {
                static_assert(sizeof(::FsGameCardErrorInfo) == sizeof(ams::fs::GameCardErrorInfo));
                R_RETURN(fsDeviceOperatorGetGameCardErrorInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardErrorInfo *>(out.GetPointer())));
            }

            /* cmd 217 */
            Result GetGameCardErrorReportInfo(ams::sf::Out<fs::GameCardErrorReportInfo> out) {
                static_assert(sizeof(::FsGameCardErrorReportInfo) == sizeof(fs::GameCardErrorReportInfo));
                R_RETURN(fsDeviceOperatorGetGameCardErrorReportInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardErrorReportInfo *>(out.GetPointer())));
            }

            /* cmd 218 */
            Result GetGameCardDeviceId(ams::sf::OutBuffer out, s64 size) {
                R_RETURN(fsDeviceOperatorGetGameCardDeviceId(std::addressof(m_operator), out.GetPointer(), out.GetSize(), size));
            }

            /* cmd 219 */
            Result ChallengeCardExistence(ams::sf::OutBuffer dst, ams::sf::InBuffer seed, ams::sf::InBuffer value, u32 handle) {
                static_assert(sizeof(::FsGameCardHandle) == sizeof(u32));
                R_RETURN(fsDeviceOperatorChallengeCardExistence(std::addressof(m_operator), reinterpret_cast<const ::FsGameCardHandle *>(std::addressof(handle)), dst.GetPointer(), dst.GetSize(), reinterpret_cast<void *>(const_cast<u8 *>(seed.GetPointer())), seed.GetSize(), reinterpret_cast<void *>(const_cast<u8 *>(value.GetPointer())), value.GetSize()));
            }

            /* cmd 220 */
            Result GetGameCardCompatibilityType(ams::sf::Out<u8> out, u32 handle) {
                R_RETURN(fsDeviceOperatorGetGameCardCompatibilityType(std::addressof(m_operator), handle, out.GetPointer()));
            }

            /* cmd 221 */
            Result GetGameCardAsicCertificate(ams::sf::OutBuffer out, s64 out_size) {
                R_RETURN(fsDeviceOperatorGetGameCardAsicCertificate(std::addressof(m_operator), reinterpret_cast<::FsGameCardAsicCertificateSet *>(out.GetPointer()), static_cast<u64>(out_size)));
            }

            /* cmd 222 */
            Result GetGameCardCardHeaderSet(ams::sf::OutBuffer out_buf, s64 out_size, u32 handle) {
                R_RETURN(fsDeviceOperatorGetGameCardCardHeader(std::addressof(m_operator), out_buf.GetPointer(), static_cast<u64>(out_size), handle));
            }

            /* cmd 223 */
            Result SetGameCardSessionCreationDelay(bool enabled, u32 delay, bool flag) {
                R_RETURN(fsDeviceOperatorSetGameCardSessionCreationDelay(std::addressof(m_operator), enabled, delay, flag));
            }

            /* cmd 224 */
            Result GetGameCardApplicationIdList(ams::sf::Out<u16> out_count, ams::sf::OutBuffer out_buf, s64 out_size, u32 handle) {
                R_RETURN(fsDeviceOperatorGetGameCardApplicationIdList(std::addressof(m_operator), out_count.GetPointer(), out_buf.GetPointer(), static_cast<u64>(out_size), handle));
            }

            /* cmd 225 */
            Result RegisterGameCardConfigurationData(ams::sf::InBuffer in_buf, s64 in_size) {
                R_RETURN(fsDeviceOperatorRegisterGameCardConfigurationData(std::addressof(m_operator), in_buf.GetPointer(), static_cast<u64>(in_size)));
            }

            /* cmd 226 */
            Result GetGameCardDetailedErrorReportInfo(ams::sf::OutBuffer out, s64 out_size) {
                R_RETURN(fsDeviceOperatorGetGameCardDetailedErrorReportInfo(std::addressof(m_operator), reinterpret_cast<::FsGameCardDetailedErrorReportInfo *>(out.GetPointer()), static_cast<u64>(out_size)));
            }

            /* cmd 300 */
            Result SetSpeedEmulationMode(u32 mode) {
                R_RETURN(fsDeviceOperatorSetSpeedEmulationMode(std::addressof(m_operator), mode));
            }

            /* cmd 301 */
            Result GetSpeedEmulationMode(ams::sf::Out<u32> out) {
                R_RETURN(fsDeviceOperatorGetSpeedEmulationMode(std::addressof(m_operator), out.GetPointer()));
            }

            /* cmd 302 */
            Result SetApplicationStorageSpeed(u32 speed) {
                R_RETURN(fsDeviceOperatorSetApplicationStorageSpeed(std::addressof(m_operator), speed));
            }

            /* cmd 303 */
            Result SetGameCardClockRateForSpeedEmulation(u32 clock_rate) {
                R_RETURN(fsDeviceOperatorSetGameCardClockRateForSpeedEmulation(std::addressof(m_operator), clock_rate));
            }

            /* cmd 304 */
            Result ClearGameCardClockRateForSpeedEmulation() {
                R_RETURN(fsDeviceOperatorClearGameCardClockRateForSpeedEmulation(std::addressof(m_operator)));
            }

            /* cmd 400 */
            Result SuspendSdmmcControl() {
                R_RETURN(fsDeviceOperatorSuspendSdmmcControl(std::addressof(m_operator)));
            }

            /* cmd 401 */
            Result ResumeSdmmcControl() {
                R_RETURN(fsDeviceOperatorResumeSdmmcControl(std::addressof(m_operator)));
            }

            /* cmd 402 */
            Result GetSdmmcConnectionStatus(ams::sf::Out<u32> out_speed_mode, ams::sf::Out<u32> out_bus_width, u32 port) {
                R_RETURN(fsDeviceOperatorGetSdmmcConnectionStatus(std::addressof(m_operator), out_speed_mode.GetPointer(), out_bus_width.GetPointer(), port));
            }

            /* cmd 500 */
            Result SetDeviceSimulationEvent(u32 device_type, u32 target_op, u32 failure_type, u32 result_value, u8 flag) {
                R_RETURN(fsDeviceOperatorSetDeviceSimulationEvent(std::addressof(m_operator), device_type, target_op, failure_type, result_value, flag));
            }

            /* cmd 501 */
            Result ClearDeviceSimulationEvent(u32 device_type) {
                R_RETURN(fsDeviceOperatorClearDeviceSimulationEvent(std::addressof(m_operator), device_type));
            }
    };
    static_assert(fssrv::sf::IsIDeviceOperator<RemoteDeviceOperator>);
    #endif

}
