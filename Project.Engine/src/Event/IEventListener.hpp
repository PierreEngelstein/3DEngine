#pragma once
#include <vector>

namespace Event
{
    class IEvent;
    /**
     * @brief Base interface for event listeners.
     * 
     */
    class IEventListener
    {
        public:
            /**
             * @brief Called when an event is received.
             * 
             * @param event : The event received.
             */
            virtual void ReceiveEvent(IEvent* event) = 0;
            virtual ~IEventListener(){}
    };
}