#include <Logging/Logger.hpp>
#include "ComponentManager.hpp"
#include "IComponent.hpp"
#include "Event/IEvent.hpp"
#include "Event/EventEntityDeleted.hpp"

namespace ECS
{
    ComponentManager::ComponentManager()
    {
        for(auto &itr : m_entity_component_lookup_table)
        {
            for(auto &itr2 : itr.second)
            {
                delete (itr2.second);
            }
        }
    }

    ComponentManager::~ComponentManager()
    {
        for(auto &itr : m_entity_component_lookup_table)
        {
            for(auto &itr2 : itr.second)
            {
                LOG_INFO("Deleting component.\n")
                delete (itr2.second);
            }
            itr.second.clear();
        }
        m_entity_component_lookup_table.clear();
    }

    void ComponentManager::ReceiveEvent(Event::IEvent* event)
    {
        if(event == nullptr)
            return;

        switch(event->type())
        {
            case EVENT_ENTITY_DELETED:
            {
                Event::EventEntityDeleted* ev = static_cast<Event::EventEntityDeleted*>(event);
                LOG_DEBUG("Receive notification of entity to delete %d from component manager.\n", ev->GetId());
                DeleteEntityFromManager(ev->GetId());
                break;
            }
            default:
                break;
        }
    }

    void ComponentManager::DeleteEntityFromManager(EntityID id)
    {
        if(m_entity_component_lookup_table.find(id) == m_entity_component_lookup_table.end())
            return;

        for(auto &i : m_entity_component_lookup_table[id])
            delete(i.second);

        m_entity_component_lookup_table.erase(id);
    }

    bool ComponentManager::HasComponent(EntityID id, const std::type_info* info)
    {
        if(m_entity_component_lookup_table.find(id) == m_entity_component_lookup_table.end())
            return false;
        return m_entity_component_lookup_table[id].find(info) != m_entity_component_lookup_table[id].end();
    }
}