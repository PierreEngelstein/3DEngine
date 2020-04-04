#pragma once

#include <vector>
#include <glm/glm.hpp>
#define NDEBUG
#include "PxPhysicsAPI.h"

class Cube
{
    private:
        unsigned int vao, vertexBuffer, textureBuffer, indexBuffer, shader;
        int modelUniform, projectionUniform, textureUniform, viewUniform;
    public:
        Cube(physx::PxPhysics *mPhysics, bool isStatic = false);
        Cube(physx::PxPhysics *mPhysics, bool isStatic, glm::vec3 position);
        Cube(physx::PxPhysics *mPhysics, bool isStatic, glm::vec3 position, glm::vec3 rotation);
        ~Cube();
        void Draw(glm::mat4 view, glm::mat4 projection);

        glm::vec3 angle;
        glm::vec3 position;

        static std::vector<glm::vec3> verticesSquare;
        static std::vector<glm::vec2> textCoordSquare;
        static std::vector<unsigned int> indicesSquare;
        physx::PxRigidActor *mActor;
};