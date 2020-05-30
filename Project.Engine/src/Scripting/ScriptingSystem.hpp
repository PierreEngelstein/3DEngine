#pragma once

#include <EntityComponentSystem/ISystem.hpp>
#include <functional>
#include <Scripting/NativeScriptComponent.hpp>
#include <External/dotnet/coreclr_delegates.h>
#include <Event/IEventListener.hpp>

namespace Scripting
{
    class ScriptingSystem : public ECS::ISystem, public Event::IEventListener
    {
    public:
        ScriptingSystem();
        ~ScriptingSystem();
        void AddSystem(EntityID id, const script& sc);
        void AddSystem(EntityID id, const std::string &scriptname);

        virtual void FirstRun() override;
        virtual void Run() override;
        virtual void LastRun() override;

        virtual void ReceiveEvent(Event::IEvent* event) override;

        void DeleteEntityFromScripting(EntityID id);
    private:
        class CSHost *host;
        component_entry_point_fn addManagedScript;
        component_entry_point_fn removeManagedScript;
        component_entry_point_fn OnInit;
        component_entry_point_fn OnUpdate;
        component_entry_point_fn OnEnding;


    };
}