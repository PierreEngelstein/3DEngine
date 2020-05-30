#include <Scripting/ScriptingSystem.hpp>
#include <Scripting/ManagedScriptComponent.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <Scripting/CSHost.h>
#include <Core/Configuration.h>
#include <Logging/Logger.hpp>
#include <Event/EventEntityDeleted.hpp>
#include <Event/EventManager.hpp>

struct ManagedScriptArgument
{
    unsigned int entityID;
    const char *name;
};

namespace Scripting
{
    ScriptingSystem::ScriptingSystem() {
        Event::EventManager::AddListener(this);
        host = new CSHost(Core::Configuration::Get().GetDotnetPath(), "resources/runtimeconfig.json");

        addManagedScript = host->LoadLibrary("ProjectGame.dll", "ProjectGame.Lib", "ProjectGame", "AddManagedScriptComponent");
        removeManagedScript = host->LoadLibrary("ProjectGame.dll", "ProjectGame.Lib", "ProjectGame", "DeleteManagedScriptComponent");
        OnInit = host->LoadLibrary("ProjectGame.dll", "ProjectGame.Lib", "ProjectGame", "OnInit");
        OnUpdate = host->LoadLibrary("ProjectGame.dll", "ProjectGame.Lib", "ProjectGame", "OnUpdate");
        OnEnding = host->LoadLibrary("ProjectGame.dll", "ProjectGame.Lib", "ProjectGame", "OnEnding");

        LOG_INFO("Loaded scripting & dotnet runtime.\n")
    }

    ScriptingSystem::~ScriptingSystem() {
        ecsengine.GetEntityManager().Foreach([&](EntityID id){
            DeleteEntityFromScripting(id);
        });
        delete host;
    }

    void ScriptingSystem::AddSystem(EntityID id, const script& sc)
    {
        if(!ecsengine.GetComponentManager().HasComponent<NativeScriptComponent>(id))
        {
            ecsengine.GetComponentManager().AddComponent<NativeScriptComponent>(id);
        }

        auto scr = ecsengine.GetComponentManager().GetComponent<NativeScriptComponent>(id);
        if(scr == nullptr) return;
        scr->m_scripts->push_back(sc);
    }

    void ScriptingSystem::AddSystem(EntityID id, const std::string& scriptname)
    {
        if(!ecsengine.GetComponentManager().HasComponent<ManagedScriptComponent>(id))
        {
            ecsengine.GetComponentManager().AddComponent<ManagedScriptComponent>(id);
        }

        auto scr = ecsengine.GetComponentManager().GetComponent<ManagedScriptComponent>(id);

        bool canAdd = true;
        for(auto i : scr->m_scripts)
        {
            if(i.name == scriptname)
            {
                canAdd = false;
                break;
            }
        }

        ManagedScriptArgument argument { id, scriptname.c_str() };

        addManagedScript(&argument, sizeof(argument));
        UnitManagedScriptComponent managed {scriptname};
        scr->m_scripts.push_back(managed);

    }

    void ScriptingSystem::FirstRun()
    {
        OnInit(nullptr, 0);
    }

    void ScriptingSystem::Run()
    {
        OnUpdate(nullptr, 0);
        ecsengine.GetEntityManager().Foreach([&](EntityID id)
        {
            NativeScriptComponent* i = ecsengine.GetComponentManager().GetComponent<NativeScriptComponent>(id);
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
        OnEnding(nullptr, 0);
    }

    void ScriptingSystem::ReceiveEvent(Event::IEvent *event)
    {
        if(event == nullptr)
            return;

        switch(event->type())
        {
            case EVENT_ENTITY_DELETED:
            {
                Event::EventEntityDeleted* ev = static_cast<Event::EventEntityDeleted*>(event);
                LOG_DEBUG("Receive notification of entity to delete %d from component manager.\n", ev->GetId());
                DeleteEntityFromScripting(ev->GetId());
                break;
            }
            default:
                break;
        }
    }

    void ScriptingSystem::DeleteEntityFromScripting(EntityID id) {
        auto scr = ecsengine.GetComponentManager().GetComponent<ManagedScriptComponent>(id);
        if(scr == nullptr) return;
        for(auto& i : scr->m_scripts) {
            LOG_DEBUG("Removing script %s from entity %d\n", i.name.c_str(), id);
            ManagedScriptArgument arg{id, i.name.c_str()};
            removeManagedScript(&arg, sizeof(arg));
        }
        scr->m_scripts.clear();
        ecsengine.GetComponentManager().DestroyComponent<ManagedScriptComponent>(id);
    }


}