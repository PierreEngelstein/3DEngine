#pragma once

#include <vector>

namespace Event
{
    class IEventListener;
    class IEvent;
    class EventManager
    {
        public:
            static void AddListener(IEventListener* listener);
            static void SendEvent(IEvent* event);
        private:
            static std::vector<IEventListener*> eventListeners;
    };
}