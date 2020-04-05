#include <EntityComponentSystem/ECSEngine.hpp>
#include <Graphics/GraphicSystem.hpp>

namespace ECS
{
    ECSEngine::ECSEngine()
    {
        m_system_manager.AddSystem<Graphics::GraphicsSystem>();
    }

    ECSEngine::~ECSEngine()
    {
        
    }

    ECSEngine& ECSEngine::Get()
    {
        static ECSEngine engine;
        return engine;
    }

    ComponentManager& ECSEngine::GetComponentManager()
    {
        return m_component_manager;
    }

    EntityManager& ECSEngine::GetEntityManager()
    {
        return m_entity_manager;
    }

    SystemManager& ECSEngine::GetSystemManager()
    {
        return m_system_manager;
    }
}