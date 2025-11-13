#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include <cstring>
#include <coreinit/memorymap.h>

namespace Memory
{
    class Reader
    {
    private:
        static uint32_t const MAX_SIZE = 1024 * 1024; // 1MB
    public:
        static std::optional<std::vector<std::byte>> readVector(uint32_t address, uint32_t size)
        {
            if (address == 0 || size == 0 || size > MAX_SIZE || static_cast<uint64_t>(address) + static_cast<uint64_t>(size) > UINT32_MAX) return std::nullopt;
            if(!OSIsAddressValid(address) || !OSIsAddressValid(address + size)) return std::nullopt;

            std::vector<std::byte> buffer(size);

            std::memcpy(buffer.data(), reinterpret_cast<void*>(address), size);

            return buffer;
        }
    };
}