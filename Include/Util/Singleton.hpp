#pragma once

namespace CC::Util
{
    template <typename T>
    class Singleton
    {
    public:
        static T& instance()
        {
            static T s;
            return s;
        }
    };
}