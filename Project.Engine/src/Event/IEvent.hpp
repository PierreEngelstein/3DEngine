#pragma once
#include <string>
#include "Types.hpp"

namespace Event
{
    /**
     * @brief Base interface for events.
     * 
     */
    class IEvent
    {
        public:
            /** This event's type. */
            virtual EventType type() = 0;
            virtual ~IEvent(){}
    };
}