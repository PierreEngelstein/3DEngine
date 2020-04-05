#pragma once

#include <vector>

namespace Event
{
    class IEventListener;
    class IEvent;

    /**
     * @brief Manages the events in the game.
     * 
     */
    class EventManager
    {
        public:
            static void AddListener(IEventListener* listener);
            static void SendEvent(IEvent* event);
        private:
            static std::vector<IEventListener*> eventListeners;
    };
}