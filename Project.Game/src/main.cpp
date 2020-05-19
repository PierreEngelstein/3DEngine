#include <Core/GLWindow.hpp>
#include <Core/GameEngine.hpp>
#include <Core/ScriptingSystem.hpp>
#include <Core/TransformComponent.hpp>
#include <Core/InputSystem.hpp>
#include <Core/CameraComponent.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <EntityComponentSystem/IEntity.hpp>
#include <Physics/PhysicsSystem.hpp>
#include <Graphics/GraphicSystem.hpp>
#include <Types.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>
#include <string>
#include "Cube.hpp"
#include "Camera.h"
#include <Logging/Logger.hpp>

int main()
{
        // Create the window
        Common::IWindow* window = new Core::GLWindow(800, 600, "Window");
        // Create the engine
        Common::IEngine* engine = new Core::GameEngine(window);
        
        // Create the graphic and input systems
        ecsengine.GetSystemManager().AddSystem<Graphics::GraphicsSystem>(window);
        Graphics::GraphicsSystem *graphics = ecsengine.GetSystemManager().GetSystem<Graphics::GraphicsSystem>();
        assert(graphics != nullptr);

        ecsengine.GetSystemManager().AddSystem<Core::InputSystem>(window);
        Core::InputSystem* input = ecsengine.GetSystemManager().GetSystem<Core::InputSystem>();
        assert(input != nullptr);
        
        // Initialize the engine
        engine->Init();
        
        // Create som objects
        Cube my_cube( true, glm::vec3(0.0f, 0.0f, 0.0f));
        Cube cube1( false, glm::vec3(0.0f, 0.0f, 5.0f));
        // Cube cube2( false, glm::vec3(0.0f, 0.0f, 0.0f));
        
        // Get the physics engine
        Physics::PhysicsSystem *phys = ecsengine.GetSystemManager().GetSystem<Physics::PhysicsSystem>();
        assert(phys != nullptr);
        
        // Get the scripting engine
        Core::ScriptingSystem *scripting = ecsengine.GetSystemManager().GetSystem<Core::ScriptingSystem>();
        assert(scripting != nullptr);
        // Add a script to the cube1
        scripting->AddSystem(cube1.m_id, [&cube1](EntityID id)
        {
                Core::TransformComponent* transf = ecsengine.GetComponentManager().GetComponent<Core::TransformComponent>(id);
        });

        Camera cam(glm::vec3(0, 1, 0), -90, 0, 120.0f);

        ecsengine.GetSystemManager().AddDependency<Core::InputSystem, Physics::PhysicsSystem>();
        ecsengine.GetSystemManager().AddDependency<Physics::PhysicsSystem, Core::ScriptingSystem>();
        ecsengine.GetSystemManager().AddDependency<Core::ScriptingSystem, Graphics::GraphicsSystem>();
        ecsengine.GetSystemManager().ProcessOrder();

        ecsengine.GetSystemManager().FirstRun();

        /** Main game loop **/
        while(window->IsRunning())
        {
                engine->OnFrameStart();

                ecsengine.GetSystemManager().Run();

                engine->OnFrameEnd();
        }

        ecsengine.GetSystemManager().LastRun();

        delete window;
        delete engine;
}