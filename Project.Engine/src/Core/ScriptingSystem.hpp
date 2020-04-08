#pragma once

#include <EntityComponentSystem/ISystem.hpp>
#include <functional>
#include <Core/ScriptComponent.hpp>

namespace Core
{
    class ScriptingSystem : public ECS::ISystem
    {
        public:
            void AddSystem(EntityID id, const script& sc);

            virtual void FirstRun() override;
            virtual void Run() override;
            virtual void LastRun() override;
    };
}