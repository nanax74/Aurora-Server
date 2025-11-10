#include "Net/Protocol/Registry.hpp"
#include "Net/Protocol/InvalidData.hpp"


namespace Net::Protocol
{
    void Registry::registerFactory(int id, std::function<IData*()> factory)
    {
        factories[id] = factory;
    }

    IData* Registry::create(int id)
    {
        auto it = factories.find(id);
        if(it != factories.end()) return it->second();
        return new InvalidData();
    }
}