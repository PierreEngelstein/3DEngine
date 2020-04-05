#pragma once

#include <EntityComponentSystem/ISystem.hpp>
#include <vector>
#include "Types.hpp"
#include "glm/glm.hpp"

namespace Graphics
{
    class GraphicsSystem : public ECS::ISystem
    {
        public:
            /** Generates the full vertex array for the wanted entity. */
            void GenerateVertexArray(EntityID id);

            void DrawEntity(EntityID id, glm::mat4 view, glm::mat4 projection);

            virtual void FirstRun() override;
            virtual void Run() override;
            virtual void LastRun() override;
        private:
            static std::vector<glm::vec3> verticesSquare;
            static std::vector<glm::vec2> textCoordSquare;
            static std::vector<unsigned int> indicesSquare;
    };
}