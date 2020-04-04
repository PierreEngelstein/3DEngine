#pragma once

#include "Event/IEvent.hpp"
#include "Types.hpp"

namespace Event
{
    /**
     * @brief Event fired when an entity has been removed from the EntityManager.
     * 
     */
    class EventEntityDeleted : public IEvent
    {
        public:
            EventEntityDeleted(EntityID id) : m_id(id)
            {

            }

            ~EventEntityDeleted()
            {
                
            }

            virtual EventType type() override
            {
                return EVENT_ENTITY_DELETED;
            }

            EntityID GetId() const
            {
                return m_id;
            }
        private:
            EntityID m_id;
    };
}