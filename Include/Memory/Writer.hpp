#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <cstring>
#include <coreinit/cache.h>
#include <coreinit/memorymap.h>

#include <kernel/kernel.h>

namespace Memory
{
    class Writer
    {
    private:
        static uint32_t const MAX_SIZE = 1024 * 1024; // 1MB
    public:
        static bool writeVector(uint32_t address, const std::vector<std::byte>& data)
        {
            uint32_t size = data.size();
            if (address == 0 || data.empty() || size > MAX_SIZE || static_cast<uint64_t>(address) + static_cast<uint64_t>(size) > UINT32_MAX) return false;
            if(!OSIsAddressValid(address) || !OSIsAddressValid(address + size)) return false;

            std::memcpy(reinterpret_cast<void*>(address), data.data(), size);

            return true;
        }

        static bool writeVectorSuper(uint32_t address, const std::vector<std::byte>& data)
        {
            uint32_t size = data.size();
            if (address == 0 || data.empty() || size > MAX_SIZE || static_cast<uint64_t>(address) + static_cast<uint64_t>(size) > UINT32_MAX) return false;
            if(!OSIsAddressValid(address) || !OSIsAddressValid(address + size)) return false;

            uint32_t dist = OSEffectiveToPhysical(address);
            uint32_t src = OSEffectiveToPhysical(reinterpret_cast<uint32_t>(data.data()));

            KernelCopyData(dist, src, size);
            DCFlushRange((void *)address, size);
            ICInvalidateRange((void *)address, size);
            
            return true;
        }
    };
}