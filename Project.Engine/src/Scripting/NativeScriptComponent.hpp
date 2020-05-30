#pragma once

#include <EntityComponentSystem/IComponent.hpp>
#include <vector>
#include <functional>
#include <Types.hpp>
#include <memory>

typedef std::function<void(EntityID)> script;

namespace Scripting
{
    class NativeScriptComponent : public ECS::IComponent
    {
        public:
            ~NativeScriptComponent()
            {
                m_scripts->clear();
                delete m_scripts;
            }
            std::vector<script> * m_scripts = new std::vector<script>;
    };
}