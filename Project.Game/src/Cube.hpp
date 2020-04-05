#pragma once

#include <vector>
#include <glm/glm.hpp>
#define NDEBUG
#include "PxPhysicsAPI.h"
#include <Graphics/MaterialComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Common/IShader.hpp>
#include <Common/ITexture.hpp>
#include "Types.hpp"

class Cube
{
    public:
        Cube(physx::PxPhysics *mPhysics, bool isStatic = false);
        Cube(physx::PxPhysics *mPhysics, bool isStatic, glm::vec3 position);
        Cube(physx::PxPhysics *mPhysics, bool isStatic, glm::vec3 position, glm::vec3 rotation);
        ~Cube();
        void Draw(glm::mat4 view, glm::mat4 projection);

        physx::PxRigidActor *mActor;

        EntityID m_id;
};