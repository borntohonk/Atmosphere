#pragma once
#include <stratosphere.hpp>
#include <stratosphere/fssrv/fssrv_interface_adapters.hpp>
#include <stratosphere/fssrv/sf/fssrv_sf_i_device_operator.hpp>
#include "../../../../libraries/libstratosphere/source/fs/impl/fs_remote_device_operator.hpp"

AMS_SF_DEFINE_MITM_INTERFACE(ams::mitm::fs, IDeviceOperatorMitmInterface, AMS_FSSRV_I_DEVICE_OPERATOR_INTERFACE_INFO, 0x1484E21C)

namespace ams::mitm::fs {

    using RemoteDeviceOperatorBase = ams::fs::impl::RemoteDeviceOperator;

    class FsRemoteDeviceOperatorMitmService : public RemoteDeviceOperatorBase {
        public:
            using RemoteDeviceOperatorBase::RemoteDeviceOperatorBase;

        public:
            Result GetGameCardCompatibilityType(ams::sf::Out<u8> out, u32 handle);
    };
    static_assert(IsIDeviceOperatorMitmInterface<FsRemoteDeviceOperatorMitmService>);

}
