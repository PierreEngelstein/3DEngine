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
#include <Logging/Logger.hpp>

EntityID camera;

EntityID GetCameraID()
{
        return camera;
}

float yaw = 0.0f;
float pitch = 0.0f;

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
//                 LOG(Cube1, Logging::Info, "cube1.transf=[%5.2f, %5.2f, %5.2f]\n", transf->m_position.x, transf->m_position.y, transf->m_position.z)
        });
        
        // Create the camera object
        camera = ecsengine.GetEntityManager().CreateEntity<ECS::IEntity>();
        ecsengine.GetComponentManager().AddComponent<Core::CameraComponent>(camera);
        auto tr = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(camera);
        tr->Position = glm::vec3(0, 1, 0);
        tr->yaw = -90;
        tr->pitch = 0;
        tr->UpVector = glm::vec3(0, 0, 1);
        tr->NearPlane = 0.1f; tr->FarPlane = 100.0f; tr->FieldOfView = 45.0f;
        scripting->AddSystem(camera, [&](EntityID id)
        {
            if(tr->yaw > 360) tr->yaw = 0.0f;
        });
        LOG(Main, Logging::Info, "camera.position.x = %5.2f\n", tr->Position.x)
        input->Subscribe("escape", camera, [&](EntityID id)
        {
            if(input->CursorState()) input->ShowCursor();
            else input->HideCursor();
        });
        input->Subscribe("forward", camera, [&](EntityID id)
        {
            tr->Position += 0.0005f * tr->Front();
        });
        input->Subscribe("backward", camera, [&](EntityID id)
        {
            tr->Position -= 0.0005f * tr->Front();
        });
        input->Subscribe("left", camera, [&](EntityID id)
        {
            tr->Position -= glm::normalize(glm::cross(tr->Front(), tr->UpVector)) * 0.0005f;
        });
        input->Subscribe("right", camera, [&](EntityID id)
        {
            tr->Position += glm::normalize(glm::cross(tr->Front(), tr->UpVector)) * 0.0005f;
        });
        input->Subscribe("mousemove", camera, [&](EntityID id)
        {
            tr->yaw -= input->DeltaX() * 0.08f;
            tr->pitch -= input->DeltaY() * 0.08f;
//            tr->Position += glm::normalize(glm::cross(tr->Front(), tr->UpVector)) * 0.05f;
        });


        input->Subscribe("test", camera, [&](EntityID id)
        {
            tr->yaw += 0.5f;
            if(tr->pitch > 80.0f) tr->pitch = 80.0f;
            if(tr->pitch < -80.0f) tr->pitch = 80.0f;
        });

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