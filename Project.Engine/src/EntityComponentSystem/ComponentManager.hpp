#pragma once

#include <map>
#include <typeinfo>
#include <memory>
#include <iostream>
#include <vector>
#include "Types.hpp"
#include "Event/IEventListener.hpp"

namespace ECS
{
    class IComponent;

    /**
     * @brief Manages all the components in the program.
     */
    class ComponentManager : public Event::IEventListener
    {
        public:
            ComponentManager();
            ~ComponentManager();

            /**
             * @brief Checks if the entity has the component requested.
             * 
             * @param id : The id of the entity to be checked.
             * @tparam C : The component type requested.
             * @return true if the entity has the component.
             * @return false otherwise.
             */
            template<class C> bool HasComponent(EntityID id)
            {
                return HasComponent(id, &typeid(C));
            }

            /**
             * @brief Adds a component to and entity.
             * 
             * @tparam C : The component type to be added.
             * @tparam arguments : Used to initialize the component.
             * @param id : The entity ID for which to add a component.
             */

            template<class C, typename... arguments>
            void AddComponent(EntityID id, arguments&&... a)
            {
                m_entity_component_lookup_table[id][&typeid(C)] = new C(a...);
            }
            
            /**
             * @brief Get the Component of type C for the entity id.
             * 
             * @tparam C : The type of component to retrieve.
             * @param id : The EntityID form which to get a component.
             * @return C* : Pointer to the component if exists.
             * @return nullptr : If no component of type C has been found for entity id.
             */
            template<class C>
            C* GetComponent(EntityID id)
            {
                if(HasComponent<C>(id))
                    return static_cast<C*>(m_entity_component_lookup_table[id][&typeid(C)]);
                else
                    return nullptr;
            }

            /**
             * @brief Destroys the component of type C for the entity id.
             * 
             * @tparam C : The type of component to destroy.
             * @param id : The EntityID for which to destroy a component.
             */
            template<class C>
            void DestroyComponent(EntityID id)
            {
                if(HasComponent<C>(id))
                {
                    auto it = m_entity_component_lookup_table[id].find(&typeid(C));
                    if(it != m_entity_component_lookup_table[id].end())
                    {
                        IComponent* cmp = it->second;
                        m_entity_component_lookup_table[id].erase(it);
                        delete cmp;
                    }else
                    {
                        std::cerr << "Entity " << id << " does not exist\n";
                    }
                }
            }

            /**
             * @brief Receives an event from the EventManager 
             * 
             * @param event : The event received.
             */
            virtual void ReceiveEvent(Event::IEvent* event) override;
        private:
            std::map<EntityID, std::map<const std::type_info*, IComponent*>> m_entity_component_lookup_table;

            /**
             * @brief Properly deletes an entity from the manager, including its components.
             * 
             * @param id : The id of the entity to be deleted.
             */
            void DeleteEntityFromManager(EntityID id);

            /**
             * @brief Checks if the entity has the component requested.
             * 
             * @param id : The id of the entity to be checked.
             * @param type : The type_info structure for the component class to be searched.
             * @return true if the entity has the component.
             * @return false otherwise.
             */
            bool HasComponent(EntityID id, const std::type_info* type);
    };
}