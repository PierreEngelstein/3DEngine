#pragma once

#include "glm/glm.hpp"
#include <EntityComponentSystem/IComponent.hpp>

namespace Core
{
    class CameraComponent : public ECS::IComponent
    {
        public:
            glm::vec3 Position;
            glm::vec3 UpVector;
            glm::vec3 Front();
            void horizontal(float speed);
            void vertical(float speed);

            float yaw;
            float pitch;

            float FieldOfView;
            float NearPlane;
            float FarPlane;
    };
}