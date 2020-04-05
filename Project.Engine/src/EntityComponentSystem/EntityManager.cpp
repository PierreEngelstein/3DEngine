#include "EntityManager.hpp"
#include <iostream>
#include "IEntity.hpp"
#include "Event/IEvent.hpp"
#include "Event/EventManager.hpp"
#include "Event/EventEntityDeleted.hpp"
#include <cassert>

namespace ECS
{
    EntityManager::EntityManager()
    {
        srand(time(NULL));
    }

    EntityManager::~EntityManager()
    {
        for(auto itr : m_entities)
        {
            for(auto itr2 : itr.second)
            {
                delete(itr2.second);
            }
        }
    }

    bool EntityManager::HasEntity(EntityID id)
    {
        for(auto itr : m_entities)
        {
            if(itr.second.find(id) != itr.second.end())
                return true;
        }
        return false;
    }

    EntityID EntityManager::GenerateID()
    {
        EntityID val = 0;
        do
        {
            val = rand();
        } while (HasEntity(val));
        return val;
    }

    void EntityManager::RemoveEntity(EntityID id)
    {
        for(auto& itr : m_entities)
        {
            auto it = itr.second.find(id);
            if(it != itr.second.end())
            {
                IEntity* ent = it->second;
                itr.second.erase(id);
                delete(ent);
                Event::EventManager::SendEvent(new Event::EventEntityDeleted(id));
                break;
            }
        }
    }

    bool EntityManager::HasEntity(EntityID id, const std::type_info* typeinfo)
    {
        if(m_entities.find(typeinfo) == m_entities.end()) // No entities of type C exist
            return false;

        if(m_entities[typeinfo].find(id) == m_entities[typeinfo].end()) // No entity id has been found in the entities of type C
            return false;
        
        return true;
    }

    void EntityManager::ReceiveEvent(Event::IEvent* event)
    {
        if(event == nullptr)
            return;
        
        switch(event->type())
        {
            default:
                break;
        }
    }

    void EntityManager::Foreach(const std::function<void(EntityID)>& __foreach__)
    {
        for(auto& i : m_entities)
        {
            for(auto& j : i.second )
            {
                __foreach__(j.first);
            }
        }
        
    }
}