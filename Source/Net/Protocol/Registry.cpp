#include "Net/Protocol/Registry.hpp"
#include "Net/Protocol/InvalidData.hpp"

#include <memory>

namespace Net::Protocol
{
    void Registry::registerFactory(ID id, std::function<std::unique_ptr<IData>(std::vector<std::byte> data)> factory)
    {
        factories[id] = factory;
    }

    std::unique_ptr<IData> Registry::create(ID id, std::vector<std::byte> data)
    {
        auto it = factories.find(id);
        if(it != factories.end()) return it->second(data);
        return std::unique_ptr<InvalidData>(new InvalidData());
    }
}