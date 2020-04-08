#include <Core/GLWindow.hpp>
#include <Core/GameEngine.hpp>
#include <Core/ScriptingSystem.hpp>
#include <Core/TransformComponent.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <EntityComponentSystem/IEntity.hpp>
#include <Physics/PhysicsSystem.hpp>
#include <Graphics/GraphicSystem.hpp>
#include <Types.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <string>
#include "Cube.hpp"

EntityID camera;

EntityID GetCameraID()
{
        return camera;
}

int main()
{
        ecsengine.GetEntityManager().CreateEntity<ECS::IEntity>();

        Common::IWindow* window = new Core::GLWindow(800, 600, "Window");
        Common::IEngine* engine = new Core::GameEngine(window);
        
        ecsengine.GetSystemManager().AddSystem<Graphics::GraphicsSystem>(window);
        Graphics::GraphicsSystem *graphics = ecsengine.GetSystemManager().GetSystem<Graphics::GraphicsSystem>();
        assert(graphics != nullptr);

        engine->Init();

        Cube my_cube( true, glm::vec3(0.0f, 0.0f, 0.0f));
        Cube cube1( false, glm::vec3(0.7f, 4.0f, 0.7f));
        Cube cube2( false, glm::vec3(0.3f, 2.0f, 0.5f));

        Physics::PhysicsSystem *phys = ecsengine.GetSystemManager().GetSystem<Physics::PhysicsSystem>();
        assert(phys != nullptr);

        Core::ScriptingSystem *scripting = ecsengine.GetSystemManager().GetSystem<Core::ScriptingSystem>();
        assert(scripting != nullptr);
        scripting->AddSystem(cube1.m_id, [&cube1](EntityID id)
        {
        });

        camera = ecsengine.GetEntityManager().CreateEntity<ECS::IEntity>();
        ecsengine.GetComponentManager().AddComponent<Core::TransformComponent>(camera);
        auto tr = ecsengine.GetComponentManager().GetComponent<Core::TransformComponent>(camera);
        tr->m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        tr->m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);

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