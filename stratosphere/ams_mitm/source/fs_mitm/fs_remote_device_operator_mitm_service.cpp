/*
 * Copyright (c) Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stratosphere.hpp>
#include "fs_remote_device_operator_mitm_service.hpp"

namespace ams::mitm::fs {

    Result FsRemoteDeviceOperatorMitmService::GetGameCardCompatibilityType(ams::sf::Out<u8> out, u32 handle) {
        AMS_UNUSED(handle);

        out.SetValue(static_cast<u8>(ams::fs::GameCardCompatibilityType::Normal));
        R_SUCCEED();
    }

}
