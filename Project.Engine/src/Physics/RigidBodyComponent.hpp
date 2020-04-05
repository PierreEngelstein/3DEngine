#pragma once

#include <EntityComponentSystem/IComponent.hpp>
#include <vector>

#define NDEBUG
#include "PxPhysicsAPI.h"

namespace Physics
{
    class RigidBodyComponent : public ECS::IComponent
    {
        public:
            physx::PxRigidActor *m_actor;
    };
}