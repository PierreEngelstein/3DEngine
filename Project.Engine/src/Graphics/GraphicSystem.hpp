#pragma once

#include <EntityComponentSystem/ISystem.hpp>
#include <vector>
#include "Types.hpp"
#include "glm/glm.hpp"

namespace Common
{
    class IWindow;
}
namespace Graphics
{
    class GraphicsSystem : public ECS::ISystem
    {
        public:
            GraphicsSystem(Common::IWindow* win);
            /** Generates the full vertex array for the wanted entity. */
            void GenerateVertexArray(EntityID id);
            
            /**
             * @brief Draws the entity on screen
             * 
             * @param id : The ID of the entity to be rendered.
             * @param view : The view matrix.
             * @param projection : The projection matrix.
             */
            void DrawEntity(EntityID id, glm::mat4 view, glm::mat4 projection);

            virtual void FirstRun() override;
            virtual void Run() override;
            virtual void LastRun() override;
        private:
            static std::vector<glm::vec3> verticesSquare;
            static std::vector<glm::vec2> textCoordSquare;
            static std::vector<unsigned int> indicesSquare;
            Common::IWindow *m_win;
            EntityID cameraId;
    };
}