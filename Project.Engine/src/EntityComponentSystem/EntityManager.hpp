#pragma once
#include <map>

#include "Types.hpp"
#include <iostream>
#include "Event/IEventListener.hpp"

namespace ECS
{
    class IEntity;

    /**
     * @brief Stores all the entities of the game.
     * 
     */
    class EntityManager : public Event::IEventListener
    {
        public:
            EntityManager();
            ~EntityManager();
            
            /**
             * @brief Removed the given entity from the manager.
             * 
             * @param id : The id of the entity to be removed.
             */
            void RemoveEntity(EntityID id);

            /**
             * @brief Create a Entity object from its class and its arguments.
             * 
             * @tparam C : The type of entity to be created.
             * @tparam arguments : The list of arguments to be used for C constructor.
             * @return EntityID : The id of the entity created.
             */
            template<class C, typename... arguments>
            EntityID CreateEntity(arguments&&... a)
            {
                EntityID ID = GenerateID();
                C *c = new C(ID, a...);
                m_entities[&typeid(C)][ID] = c;
                return ID;
            }

            /**
             * @brief Get the Entity object from its id
             * 
             * @tparam C : The type of entity we want.
             * @param id : The id of the entity wanted.
             * @return C* : The pointer to the entity if it exists, nullptr otherwise.
             */
            template<class C>
            C* GetEntity(EntityID id)
            {
                if(!HasEntity(id, &typeid(C)))
                    return nullptr;

                return static_cast<C*>(m_entities[&typeid(C)][id]);
            }

            /**
             * @brief Checks if an entity of type C that has the given id exists.
             * 
             * @tparam C : The type of entity to be searched.
             * @param id : The id of the entity to be searched.
             * @return true if it exists.
             * @return false if it does not exist.
             */
            template<class C>
            bool HasEntity(EntityID id)
            {
                return HasEntity(id, &typeid(C));
            }

            /**
             * @brief Receives an event from the EventManager 
             * 
             * @param event : The event received.
             */
            virtual void ReceiveEvent(Event::IEvent* event) override;
            
        private:
            /**
             * One map per entity type (prevents to cast from a derived type to another)
             * 
             */
            std::map<const std::type_info *, std::map<EntityID, IEntity*>> m_entities;

            /**
             * @brief Generates a new ID that does not exist in the game.
             * 
             * @return The generated id.
             */
            EntityID GenerateID();

            /**
             * @brief Checks if an entity of type_info that has the given id exists.
             * 
             * @tparam typeinfo : The type_info of entity to be searched.
             * @param id : The id of the entity to be searched.
             * @return true if it exists.
             * @return false if it does not exist.
             */
            bool HasEntity(EntityID id, const std::type_info* typeinfo);

            /**
             * @brief Checks if an entity that has the given id exists.
             * 
             * @param id : The id of the entity to be searched.
             * @return true if it exists.
             * @return false if it does not exist.
             */
            bool HasEntity(EntityID id);
    };
}
