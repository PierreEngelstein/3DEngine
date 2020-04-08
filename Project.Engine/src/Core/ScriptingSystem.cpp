#include <Core/ScriptingSystem.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>


namespace Core
{
    void ScriptingSystem::AddSystem(EntityID id, const script& sc)
    {
        if(!ecsengine.GetComponentManager().HasComponent<ScriptComponent>(id))
        {
            ecsengine.GetComponentManager().AddComponent<ScriptComponent>(id);
        }

        auto scr = ecsengine.GetComponentManager().GetComponent<ScriptComponent>(id);
        if(scr == nullptr) return;
        scr->m_scripts->push_back(sc);
    }

    void ScriptingSystem::FirstRun()
    {

    }

    void ScriptingSystem::Run()
    {
        ecsengine.GetEntityManager().Foreach([&](EntityID id)
        {
            ScriptComponent* i = ecsengine.GetComponentManager().GetComponent<ScriptComponent>(id);
            if(i != nullptr)
            {
                for(auto j = i->m_scripts->begin(); j != i->m_scripts->end(); j++)
                {
                    (*j)(id);
                }
            }
        });
    }

    void ScriptingSystem::LastRun()
    {

    }
}