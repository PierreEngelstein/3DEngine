#include <Core/GLWindow.hpp>
#include <Core/GameEngine.hpp>
#include <Scripting/ScriptingSystem.hpp>
#include <Core/TransformComponent.hpp>
#include <Core/InputSystem.hpp>
#include <Logging/Logger.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <EntityComponentSystem/IEntity.hpp>
#include <Physics/PhysicsSystem.hpp>
#include <Graphics/GraphicSystem.hpp>
#include <Core/Configuration.h>
#include <Types.hpp>

#include "glm/glm.hpp"

#include <string>
#include "Cube.hpp"
#include "Camera.h"

int main(int argc, const char **argv)
{
    std::string st = Core::Configuration::Get(argv).GetDotnetPath();
    std::cout << "dotnet path = " << st << "\n";
    Logging::SetMinimalVerbosity(Logging::Debug);
    Logging::SetMaximalVerbosity(Logging::Fatal);
    Logging::UseCompleteName(true);
//    Logging::SetFilterCategory("Graphics.*");
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

    // Create some objects
    Cube my_cube( true, glm::vec3(0.0f, 0.0f, 0.0f));
    Cube cube1( false, glm::vec3(0.0f, 0.0f, 5.0f));
    // Cube cube2( false, glm::vec3(0.0f, 0.0f, 0.0f));

    // Get the physics engine
    Physics::PhysicsSystem *phys = ecsengine.GetSystemManager().GetSystem<Physics::PhysicsSystem>();
    assert(phys != nullptr);

    // Get the scripting engine
    Scripting::ScriptingSystem *scripting = ecsengine.GetSystemManager().GetSystem<Scripting::ScriptingSystem>();
    assert(scripting != nullptr);
    // Add a script to the cube1
    scripting->AddSystem(cube1.m_id, [&cube1](EntityID id)
    {
        Core::TransformComponent* transf = ecsengine.GetComponentManager().GetComponent<Core::TransformComponent>(id);
    });

    input->Subscribe("forward", cube1.m_id, [&](EntityID id)
    {
        scripting->DeleteEntityFromScripting(cube1.m_id);
    });

    scripting->AddSystem(cube1.m_id, "myscript");

    Camera cam(glm::vec3(0, 1, 0), -90, 0, 120.0f, false);

    ecsengine.GetSystemManager().AddDependency<Core::InputSystem, Physics::PhysicsSystem>();
    ecsengine.GetSystemManager().AddDependency<Physics::PhysicsSystem, Scripting::ScriptingSystem>();
    ecsengine.GetSystemManager().AddDependency<Scripting::ScriptingSystem, Graphics::GraphicsSystem>();
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