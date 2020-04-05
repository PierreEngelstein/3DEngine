#include <Core/GLWindow.hpp>
#include <Core/GameEngine.hpp>
#include <Graphics/GLRenderer.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <EntityComponentSystem/IEntity.hpp>
#include <Physics/PhysicsSystem.hpp>
#include <Graphics/GraphicSystem.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <string>
#include "Cube.hpp"

int main()
{
        ecsengine.GetEntityManager().CreateEntity<ECS::IEntity>();

        Common::IWindow* window = new Core::GLWindow(800, 600, "Window");
        Common::IRenderer* renderer = new Graphics::GLRenderer();
        Common::IEngine* engine = new Core::GameEngine(window, renderer);

        engine->Init();

        Cube my_cube( true, glm::vec3(0.0f, 0.0f, 0.0f));
        Cube cube1( false, glm::vec3(0.7f, 3.0f, 0.7f));

        Physics::PhysicsSystem *phys = ecsengine.GetSystemManager().GetSystem<Physics::PhysicsSystem>();
        assert(phys != nullptr);

        Graphics::GraphicsSystem *graphics = ecsengine.GetSystemManager().GetSystem<Graphics::GraphicsSystem>();
        assert(phys != nullptr);

        /** Main game loop **/
        while(window->IsRunning())
        {
                phys->Run();

                engine->OnFrameStart();

                ecsengine.GetEntityManager().Foreach([&](EntityID id)
                {
                        phys->UpdateTransformFromSimulation(id);
                });

                glm::mat4 view = glm::mat4(1.0f);
                view = glm::translate(view, glm::vec3(0.0f, -1.0f, -6.0f));
                view = glm::rotate(view, 20.0f, glm::vec3(-1.0f, 0.0f, 0.0f));

                // Build the projection matrix
                glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window->Width()/(float)window->Height(), 0.1f, 100.0f);

                ecsengine.GetEntityManager().Foreach([&](EntityID id)
                {
                        graphics->DrawEntity(id, view, projection);
                });

                engine->OnFrameEnd();
        }

        delete window;
        delete renderer;
        delete engine;
}