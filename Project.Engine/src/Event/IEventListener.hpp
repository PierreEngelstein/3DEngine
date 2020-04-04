#pragma once
#include <vector>

namespace Event
{
    class IEvent;
    class IEventListener
    {
        public:
            virtual void ReceiveEvent(IEvent* event) = 0;
    };
}