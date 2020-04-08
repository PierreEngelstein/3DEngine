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
            /**
             * @brief Adds a listener to be notified on events.
             * 
             * @param listener : The listener to be added
             */
            static void AddListener(IEventListener* listener);

            /**
             * @brief Sends an event to the listeners
             * 
             * @param event : The event to be sent.
             */
            static void SendEvent(IEvent* event);
        private:
            static std::vector<IEventListener*> eventListeners;
    };
}