#include <vector>
#include <cstddef>
#include <cstring>
#include <stdexcept>

class ByteReader
{
public:
    ByteReader(const std::vector<std::byte>& data) : data(data), pos(0) {}

    template<typename T>
    T read()
    {
        if (pos + sizeof(T) > data.size())
            throw std::out_of_range("ByteReader: out of range");

        T value;
        std::memcpy(&value, data.data() + pos, sizeof(T));
        pos += sizeof(T);
        return value;
    }

    std::vector<std::byte> readBytes(std::size_t count)
    {
        if (pos + count > data.size()) throw std::out_of_range("ByteReader: out of range");

        std::vector<std::byte> out(data.begin() + pos, data.begin() + pos + count);
        pos += count;
        return out;
    }

    std::string readString(std::size_t count)
    {
        auto bytes = readBytes(count);
        return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    }

    void skip(std::size_t count)
    {
        if (pos + count > data.size()) throw std::out_of_range("ByteReader: out of range");
        pos += count;
    }

    std::size_t position() const { return pos; }
    bool eof() const { return pos >= data.size(); }

private:
    const std::vector<std::byte>& data;
    std::size_t pos;
};
