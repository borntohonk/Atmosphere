/*
 * Copyright (c) 2018-2020 Atmosphère-NX
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
#include <mesosphere/arch/arm64/kern_cpu_system_registers.hpp>

namespace ams::kern::arm64::cpu {

#if defined(ATMOSPHERE_CPU_ARM_CORTEX_A57) || defined(ATMOSPHERE_CPU_ARM_CORTEX_A53)
    constexpr inline size_t InstructionCacheLineSize = 0x40;
    constexpr inline size_t DataCacheLineSize        = 0x40;
#else
    #error "Unknown CPU for cache line sizes"
#endif

#if defined(ATMOSPHERE_BOARD_NINTENDO_SWITCH)
    static constexpr size_t NumCores = 4;
#else
    #error "Unknown Board for cpu::NumCores"
#endif

    /* Helpers for managing memory state. */
    ALWAYS_INLINE void DataSynchronizationBarrier() {
        __asm__ __volatile__("dsb sy" ::: "memory");
    }

    ALWAYS_INLINE void DataSynchronizationBarrierInnerShareable() {
        __asm__ __volatile__("dsb ish" ::: "memory");
    }

    ALWAYS_INLINE void DataMemoryBarrier() {
        __asm__ __volatile__("dmb sy" ::: "memory");
    }

    ALWAYS_INLINE void InstructionMemoryBarrier() {
        __asm__ __volatile__("isb" ::: "memory");
    }

    ALWAYS_INLINE void EnsureInstructionConsistency() {
        DataSynchronizationBarrier();
        InstructionMemoryBarrier();
    }

    ALWAYS_INLINE void InvalidateEntireInstructionCache() {
        __asm__ __volatile__("ic iallu" ::: "memory");
        EnsureInstructionConsistency();
    }

    /* Helper for address access. */
    ALWAYS_INLINE bool GetPhysicalAddressWritable(KPhysicalAddress *out, KVirtualAddress addr, bool privileged = false) {
        const uintptr_t va = GetInteger(addr);

        if (privileged) {
            __asm__ __volatile__("at s1e1w, %[va]" :: [va]"r"(va) : "memory");
        } else {
            __asm__ __volatile__("at s1e0w, %[va]" :: [va]"r"(va) : "memory");
        }
        InstructionMemoryBarrier();

        u64 par = GetParEl1();

        if (par & 0x1) {
            return false;
        }

        if (out) {
            *out = KPhysicalAddress((par & 0xFFFFFFFFF000ull) | (va & 0xFFFull));
        }
        return true;
    }

    ALWAYS_INLINE bool GetPhysicalAddressReadable(KPhysicalAddress *out, KVirtualAddress addr, bool privileged = false) {
        const uintptr_t va = GetInteger(addr);

        if (privileged) {
            __asm__ __volatile__("at s1e1r, %[va]" :: [va]"r"(va) : "memory");
        } else {
            __asm__ __volatile__("at s1e0r, %[va]" :: [va]"r"(va) : "memory");
        }
        InstructionMemoryBarrier();

        u64 par = GetParEl1();

        if (par & 0x1) {
            return false;
        }

        if (out) {
            *out = KPhysicalAddress((par & 0xFFFFFFFFF000ull) | (va & 0xFFFull));
        }
        return true;
    }

    /* Synchronization helpers. */
    NOINLINE void SynchronizeAllCores();

    /* Cache management helpers. */
    void ClearPageToZeroImpl(void *);
    void FlushEntireDataCacheShared();
    void FlushEntireDataCacheLocal();

    ALWAYS_INLINE void ClearPageToZero(void *page) {
        MESOSPHERE_ASSERT(util::IsAligned(reinterpret_cast<uintptr_t>(page), PageSize));
        MESOSPHERE_ASSERT(page != nullptr);
        ClearPageToZeroImpl(page);
    }

    ALWAYS_INLINE void InvalidateEntireTlb() {
        __asm__ __volatile__("tlbi vmalle1is" ::: "memory");
        EnsureInstructionConsistency();
    }

    ALWAYS_INLINE uintptr_t GetCoreLocalRegionAddress() {
        register uintptr_t x18 asm("x18");
        __asm__ __volatile__("" : [x18]"=r"(x18));
        return x18;
    }

    ALWAYS_INLINE void SetCoreLocalRegionAddress(uintptr_t value) {
        register uintptr_t x18 asm("x18") = value;
        __asm__ __volatile__("":: [x18]"r"(x18));
        SetTpidrEl1(value);
    }

    ALWAYS_INLINE void SwitchThreadLocalRegion(uintptr_t tlr) {
        cpu::SetTpidrRoEl0(tlr);
    }

}
