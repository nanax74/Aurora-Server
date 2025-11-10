#pragma once

#include "Net/Protocol/IData.hpp"

#include <map>
#include <functional>
#include <memory>

namespace Net::Protocol
{
    class Registry
    {
    private:
        std::map<ID, std::function<std::unique_ptr<IData>(std::vector<std::byte> data)>> factories;
    public:
        void registerFactory(ID id, std::function<std::unique_ptr<IData>(std::vector<std::byte> data)> factory);

        std::unique_ptr<IData> create(ID id, std::vector<std::byte> data);
    };
}