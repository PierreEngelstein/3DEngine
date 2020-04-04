#pragma once
#include <string>
#include "Types.hpp"

namespace Event
{
    class IEvent
    {
        public:
            virtual EventType type() = 0;
    };
}