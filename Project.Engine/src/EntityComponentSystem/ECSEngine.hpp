#pragma once

#include <EntityComponentSystem/ComponentManager.hpp>
#include <EntityComponentSystem/EntityManager.hpp>
#include <EntityComponentSystem/SystemManager.hpp>

#define ecsengine ECS::ECSEngine::Get()

namespace ECS
{
    /**
     * @brief The singleton class managing all the ECS for the engine.
     * 
     */
    class ECSEngine
    {
        public:
            ~ECSEngine();
            ECSEngine(const ECSEngine &) = delete;
            void operator=(const ECSEngine &) = delete;
            
            /** Get the singleton instance. */
            static ECSEngine& Get();

            ComponentManager& GetComponentManager();
            EntityManager&    GetEntityManager();
            SystemManager&    GetSystemManager();
        
        private:
            ECSEngine();
            ComponentManager m_component_manager;
            EntityManager    m_entity_manager;
            SystemManager    m_system_manager;
    };
    
}