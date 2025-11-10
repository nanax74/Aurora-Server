#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <type_traits>

class ByteWriter
{
private:
    std::vector<std::byte> buffer;

public:
    const std::vector<std::byte>& data() const { return buffer; }

    void clear() { buffer.clear(); }

    template<typename T>
    void write(const T& value)
    {
        static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
        const std::byte* ptr = reinterpret_cast<const std::byte*>(&value);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
    }

    void writeBytes(const void* data, std::size_t size)
    {
        const std::byte* ptr = reinterpret_cast<const std::byte*>(data);
        buffer.insert(buffer.end(), ptr, ptr + size);
    }

    void writeString(const std::string& str)
    {
        // 長さ + 本体の順で書く
        uint32_t len = static_cast<uint32_t>(str.size());
        write(len);
        writeBytes(str.data(), len);
    }

    void writeVector(const std::vector<std::byte>& data)
    {
        uint32_t len = static_cast<uint32_t>(data.size());
        write(len);
        buffer.insert(buffer.end(), data.begin(), data.end());
    }
};
