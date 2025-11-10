#pragma once

#include "Net/Protocol/IData.hpp"

#include <map>
#include <functional>

namespace Net::Protocol
{
    class Registry
    {
    private:
        std::map<int, std::function<IData*()>> factories;
    public:
        void registerFactory(int id, std::function<IData*()> factory);

        IData* create(int id);
    };
}