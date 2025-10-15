#pragma once

namespace Aurora::Util
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