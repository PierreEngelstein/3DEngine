#ifndef INC_3DENGINE_MANAGEDSCRIPTCOMPONENT_H
#define INC_3DENGINE_MANAGEDSCRIPTCOMPONENT_H

#include <EntityComponentSystem/IComponent.hpp>
#include <External/dotnet/coreclr_delegates.h>
#include <vector>
#include <string>

namespace Scripting
{
    class UnitManagedScriptComponent
    {
        public:
            std::string name;
    };

    class ManagedScriptComponent : public ECS::IComponent
    {
        public:
            std::vector<UnitManagedScriptComponent> m_scripts;
    };
}

#endif
