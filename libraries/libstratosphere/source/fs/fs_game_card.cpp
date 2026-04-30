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
#include "fsa/fs_mount_utils.hpp"
#include "impl/fs_file_system_proxy_service_object.hpp"
#include "impl/fs_file_system_service_object_adapter.hpp"

namespace ams::fs {

    namespace {

        const char *GetGameCardMountNameSuffix(GameCardPartition which) {
            switch (which) {
                case GameCardPartition::Update: return impl::GameCardFileSystemMountNameUpdateSuffix;
                case GameCardPartition::Normal: return impl::GameCardFileSystemMountNameNormalSuffix;
                case GameCardPartition::Secure: return impl::GameCardFileSystemMountNameSecureSuffix;
                AMS_UNREACHABLE_DEFAULT_CASE();
            }
        }

        class GameCardCommonMountNameGenerator : public fsa::ICommonMountNameGenerator, public impl::Newable {
            private:
                const GameCardHandle m_handle;
                const GameCardPartition m_partition;
            public:
                explicit GameCardCommonMountNameGenerator(GameCardHandle h, GameCardPartition p) : m_handle(h), m_partition(p) { /* ... */ }

                virtual Result GenerateCommonMountName(char *dst, size_t dst_size) override {
                    /* Determine how much space we need. */
                    const size_t needed_size = util::Strnlen(impl::GameCardFileSystemMountName, MountNameLengthMax) + util::Strnlen(GetGameCardMountNameSuffix(m_partition), MountNameLengthMax) + sizeof(GameCardHandle) * 2 + 2;
                    AMS_ABORT_UNLESS(dst_size >= needed_size);

                    /* Generate the name. */
                    const auto size = util::SNPrintf(dst, dst_size, "%s%s%08x:", impl::GameCardFileSystemMountName, GetGameCardMountNameSuffix(m_partition), m_handle);
                    AMS_ASSERT(static_cast<size_t>(size) == needed_size - 1);
                    AMS_UNUSED(size);

                    R_SUCCEED();
                }
        };

    }

    Result GetGameCardHandle(GameCardHandle *out) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the handle. */
        u32 handle;
        AMS_FS_R_TRY(device_operator->GetGameCardHandle(std::addressof(handle)));

        *out = handle;
        R_SUCCEED();
    }

    Result MountGameCardPartition(const char *name, GameCardHandle handle, GameCardPartition partition) {
        auto mount_impl = [=]() -> Result {
            /* Validate the mount name. */
            R_TRY(impl::CheckMountNameAllowingReserved(name));

            /* Open the gamecard filesystem. */
            auto fsp = impl::GetFileSystemProxyServiceObject();
            sf::SharedPointer<fssrv::sf::IFileSystem> fs;
            R_TRY(fsp->OpenGameCardFileSystem(std::addressof(fs), static_cast<u32>(handle), static_cast<u32>(partition)));

            /* Allocate a new filesystem wrapper. */
            auto fsa = std::make_unique<impl::FileSystemServiceObjectAdapter>(std::move(fs));
            R_UNLESS(fsa != nullptr, fs::ResultAllocationMemoryFailedInGameCardC());

            /* Allocate a new mountname generator. */
            auto generator = std::make_unique<GameCardCommonMountNameGenerator>(handle, partition);
            R_UNLESS(generator != nullptr, fs::ResultAllocationMemoryFailedInGameCardD());

            /* Register. */
            R_RETURN(fsa::Register(name, std::move(fsa), std::move(generator)));
        };

        /* Perform the mount. */
        AMS_FS_R_TRY(AMS_FS_IMPL_ACCESS_LOG_SYSTEM_MOUNT(mount_impl(), name, AMS_FS_IMPL_ACCESS_LOG_FORMAT_MOUNT_GAME_CARD_PARTITION(name, handle, partition)));

        /* Enable access logging. */
        AMS_FS_IMPL_ACCESS_LOG_SYSTEM_FS_ACCESSOR_ENABLE(name);

        R_SUCCEED();
    }

    bool IsGameCardInserted() {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_ABORT_UNLESS(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get insertion status. */
        bool inserted;
        AMS_FS_R_ABORT_UNLESS(device_operator->IsGameCardInserted(std::addressof(inserted)));

        return inserted;
    }

    Result GetGameCardCid(void *dst, size_t size) {
        /* Check pre-conditions. */
        AMS_FS_R_UNLESS(size >= sizeof(gc::GameCardIdSet), fs::ResultInvalidSize());

        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the id set. */
        gc::GameCardIdSet gc_id_set;
        AMS_FS_R_TRY(device_operator->GetGameCardIdSet(sf::OutBuffer(std::addressof(gc_id_set), sizeof(gc_id_set)), static_cast<s64>(sizeof(gc_id_set))));

        /* Copy the id set to output. */
        std::memcpy(dst, std::addressof(gc_id_set), sizeof(gc_id_set));

        R_SUCCEED();

    }

    Result GetGameCardIdSet(gc::GameCardIdSet *out) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the id set. */
        gc::GameCardIdSet gc_id_set;
        AMS_FS_R_TRY(device_operator->GetGameCardIdSet(sf::OutBuffer(std::addressof(gc_id_set), sizeof(gc_id_set)), static_cast<s64>(sizeof(gc_id_set))));

        *out = gc_id_set;
        R_SUCCEED();
    }

    Result GetGameCardDeviceId(void *dst, size_t size) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the cid. */
        AMS_FS_R_TRY(device_operator->GetGameCardDeviceId(sf::OutBuffer(dst, size), static_cast<s64>(size)));

        R_SUCCEED();
    }

    Result GetGameCardDeviceIdForProdCard(void *dst, size_t size, const void *card_header, size_t card_header_size) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the device id for a prod card. */
        AMS_FS_R_TRY(device_operator->GetGameCardDeviceIdForProdCard(sf::OutBuffer(dst, size), static_cast<s64>(size), sf::InBuffer(card_header, card_header_size), static_cast<s64>(card_header_size)));

        R_SUCCEED();
    }

    Result GetGameCardDeviceCertificate(void *dst, size_t size, GameCardHandle handle) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the device certificate. */
        s64 certificate_size = 0;
        AMS_FS_R_TRY(device_operator->GetGameCardDeviceCertificate(std::addressof(certificate_size), sf::OutBuffer(dst, size), static_cast<s64>(size), handle));

        R_SUCCEED();
    }

    Result GetGameCardDeviceCertificate(s64 *out_size, void *dst, size_t size, GameCardHandle handle) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the device certificate. */
        AMS_FS_R_TRY(device_operator->GetGameCardDeviceCertificate(out_size, sf::OutBuffer(dst, size), static_cast<s64>(size), handle));

        R_SUCCEED();
    }

    Result GetGameCardAsicInfo(GameCardRmaInformation *out, const void *fw_buffer, size_t fw_buffer_size) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the asic info. */
        GameCardRmaInformation rma_info = {};
        AMS_FS_R_TRY(device_operator->GetGameCardAsicInfo(sf::OutBuffer(std::addressof(rma_info), sizeof(rma_info)), static_cast<s64>(sizeof(rma_info)), sf::InBuffer(fw_buffer, fw_buffer_size), static_cast<s64>(fw_buffer_size)));

        *out = rma_info;
        R_SUCCEED();
    }

    Result GetGameCardAsicCertificate(GameCardAsicCertificateSet *out) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the asic certificate. */
        GameCardAsicCertificateSet certificate = {};
        AMS_FS_R_TRY(device_operator->GetGameCardAsicCertificate(sf::OutBuffer(std::addressof(certificate), sizeof(certificate)), static_cast<s64>(sizeof(certificate))));

        *out = certificate;
        R_SUCCEED();
    }

    Result ChallengeCardExistence(void *dst, size_t size, const void *seed, size_t seed_size, const void *value, size_t value_size, GameCardHandle handle) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Challenge the card existence. */
        AMS_FS_R_TRY(device_operator->ChallengeCardExistence(sf::OutBuffer(dst, size), sf::InBuffer(seed, seed_size), sf::InBuffer(value, value_size), handle));

        R_SUCCEED();
    }

    Result GetGameCardImageHash(void *dst, size_t size, GameCardHandle handle) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the image hash. */
        AMS_FS_R_TRY(device_operator->GetGameCardImageHash(sf::OutBuffer(dst, size), handle));

        R_SUCCEED();
    }

    Result GetGameCardApplicationIdList(u16 *out_count, void *dst, size_t size, GameCardHandle handle) {
        constexpr size_t ApplicationIdListBufferSizeMin = 0x600;

        /* Check pre-conditions. */
        AMS_FS_R_UNLESS(size >= ApplicationIdListBufferSizeMin, fs::ResultInvalidSize());

        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the application id list. */
        u16 count = 0;
        AMS_FS_R_TRY(device_operator->GetGameCardApplicationIdList(std::addressof(count), sf::OutBuffer(dst, size), static_cast<s64>(size), handle));

        *out_count = count;
        R_SUCCEED();
    }

    Result GetGameCardCardHeaderSet(void *dst, size_t size, GameCardHandle handle) {
        constexpr size_t CardHeaderSetSize = 0x600;

        /* Check pre-conditions. */
        AMS_FS_R_UNLESS(size == CardHeaderSetSize, fs::ResultInvalidSize());

        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the card header set. */
        u8 card_header[CardHeaderSetSize] = {};
        AMS_FS_R_TRY(device_operator->GetGameCardCardHeaderSet(sf::OutBuffer(card_header, sizeof(card_header)), static_cast<s64>(sizeof(card_header)), handle));

        std::memcpy(dst, card_header, sizeof(card_header));
        R_SUCCEED();
    }

    Result GetGameCardAttribute(GameCardAttribute *out, GameCardHandle handle) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the attribute. */
        u8 attribute;
        AMS_FS_R_TRY(device_operator->GetGameCardAttribute(std::addressof(attribute), handle));

        *out = static_cast<GameCardAttribute>(attribute);
        R_SUCCEED();
    }

    Result GetGameCardCompatibilityType(GameCardCompatibilityType *out, GameCardHandle handle) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the compatibility type. */
        u8 type;
        AMS_FS_R_TRY(device_operator->GetGameCardCompatibilityType(std::addressof(type), handle));

        *out = static_cast<GameCardCompatibilityType>(type);
        R_SUCCEED();
    }

    Result GetGameCardUpdatePartitionInfo(bool *out_exists, GameCardUpdatePartitionInfo *out, GameCardHandle handle) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the update partition info. */
        GameCardUpdatePartitionInfo info = {};
        AMS_FS_R_TRY(device_operator->GetGameCardUpdatePartitionInfo(std::addressof(info.version), std::addressof(info.id), handle));

        /* Get the card header set. */
        constexpr size_t CardHeaderSetSize = 0x600;
        constexpr size_t UpdatePartitionHashOffset = 0x1B8;
        static_assert(UpdatePartitionHashOffset + sizeof(u64) <= CardHeaderSetSize);

        u8 card_header[CardHeaderSetSize] = {};
        AMS_FS_R_TRY(device_operator->GetGameCardCardHeaderSet(sf::OutBuffer(card_header, sizeof(card_header)), static_cast<s64>(sizeof(card_header)), handle));

        u64 update_partition_hash_prefix;
        std::memcpy(std::addressof(update_partition_hash_prefix), card_header + UpdatePartitionHashOffset, sizeof(update_partition_hash_prefix));

        *out_exists = update_partition_hash_prefix != 0;
        out->version = info.version;
        out->reserved = info.reserved;
        out->id = info.id;
        R_SUCCEED();
    }

    Result GetGameCardErrorInfo(GameCardErrorInfo *out) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the error info. */
        AMS_FS_R_TRY(device_operator->GetGameCardErrorInfo(out));
        R_SUCCEED();
    }

    Result GetGameCardErrorReportInfo(GameCardErrorReportInfo *out) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the error report info. */
        AMS_FS_R_TRY(device_operator->GetGameCardErrorReportInfo(out));

        R_SUCCEED();
    }

    Result GetGameCardDetailedErrorReportInfo(GameCardDetailedErrorReportInfo *out) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Get the detailed error report info. */
        AMS_FS_R_TRY(device_operator->GetGameCardDetailedErrorReportInfo(sf::OutBuffer(out, sizeof(*out)), static_cast<s64>(sizeof(*out))));

        R_SUCCEED();
    }

    Result EraseGameCard(u32 game_card_size, u64 normal_area_size) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Erase the game card. */
        AMS_FS_R_TRY(device_operator->EraseGameCard(game_card_size, normal_area_size));

        R_SUCCEED();
    }

    Result ForceEraseGameCard() {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Force erase the game card. */
        AMS_FS_R_TRY(device_operator->ForceEraseGameCard());

        R_SUCCEED();
    }

    void FinalizeGameCardDriver() {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_ABORT_UNLESS(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Finalize the game card driver. */
        AMS_FS_R_ABORT_UNLESS(device_operator->FinalizeGameCardDriver());
    }

    Result SetGameCardSessionCreationDelay(bool enabled, u32 minimum_delay_micro_seconds, bool forced) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Set the session creation delay. */
        AMS_FS_R_TRY(device_operator->SetGameCardSessionCreationDelay(enabled, minimum_delay_micro_seconds, forced));

        R_SUCCEED();
    }

    Result SetGameCardClockRateForSpeedEmulation(s32 clock_rate) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Set the clock rate. */
        AMS_FS_R_TRY(device_operator->SetGameCardClockRateForSpeedEmulation(clock_rate));

        R_SUCCEED();
    }

    Result ClearGameCardClockRateForSpeedEmulation() {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Clear the clock rate. */
        AMS_FS_R_TRY(device_operator->ClearGameCardClockRateForSpeedEmulation());

        R_SUCCEED();
    }

    Result RegisterGameCardConfigurationData(const void *data, size_t size) {
        auto fsp = impl::GetFileSystemProxyServiceObject();

        /* Open a device operator. */
        sf::SharedPointer<fssrv::sf::IDeviceOperator> device_operator;
        AMS_FS_R_TRY(fsp->OpenDeviceOperator(std::addressof(device_operator)));

        /* Register the configuration data. */
        AMS_FS_R_TRY(device_operator->RegisterGameCardConfigurationData(sf::InBuffer(data, size), static_cast<s64>(size)));

        R_SUCCEED();
    }

}
