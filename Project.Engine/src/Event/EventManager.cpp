#include "EventManager.hpp"
#include "IEvent.hpp"
#include "IEventListener.hpp"

namespace Event
{
    std::vector<IEventListener*> EventManager::eventListeners;

    void EventManager::AddListener(IEventListener* listener)
    {
        if(listener != nullptr)
            eventListeners.push_back(listener);
    }

    void EventManager::SendEvent(IEvent* event)
    {
        for(auto i : eventListeners)
        {
            if(i != nullptr)
            {
                i->ReceiveEvent(event);
            }
        }

        delete(event);
    }
}