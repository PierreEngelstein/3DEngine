#pragma once
#include "glm/glm.hpp"
#include <EntityComponentSystem/IComponent.hpp>

namespace Core
{
    class TransformComponent : public ECS::IComponent
    {
        public:
            glm::vec3 m_position;
            glm::vec3 m_rotation;
    };
}