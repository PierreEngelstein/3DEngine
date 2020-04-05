#include "GL/glew.h"

#include <Core/GLWindow.hpp>
#include <Core/GameEngine.hpp>
#include <Graphics/GLRenderer.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <EntityComponentSystem/IEntity.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Cube.hpp"
#include "lodepng.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define NDEBUG
#include "PxPhysicsAPI.h"

using namespace physx;

int main()
{
        ecsengine.GetEntityManager().CreateEntity<ECS::IEntity>();
        std::cout << "Hello World\n";

        Common::IWindow* window = new Core::GLWindow(800, 600, "Window");
        Common::IRenderer* renderer = new Graphics::GLRenderer();
        Common::IEngine* engine = new Core::GameEngine(window, renderer);

        engine->Init();

        // Setup PhysX
        static physx::PxDefaultAllocator gDefaultAllocator;
        static PxDefaultErrorCallback gDefaultErrorCallback;
        physx::PxFoundation *mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocator, gDefaultErrorCallback);
        if(!mFoundation)
        {
                std::cerr << "PxFoundation creation failed !\n";
                return 0;
        }

        PxPvd* mPvd = physx::PxCreatePvd(*mFoundation);
        PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
        
        PxPhysics* mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true, mPvd);
        if(!mPhysics)
        {
                std::cerr << "PxCreatePhysics failed !\n";
                return 0;
        }
        if(!PxInitExtensions(*mPhysics, mPvd))
        {
                std::cerr << "PxInitExtensions failed !\n";
                return 0;
        }

        // Create a physx scene and assign a correct gravity
        PxScene *mScene;
        PxSceneDesc mSceneDesc(mPhysics->getTolerancesScale());
        mSceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        mSceneDesc.filterShader = PxDefaultSimulationFilterShader;
        PxDefaultCpuDispatcher *mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
        if(!mCpuDispatcher)
        {
                std::cerr << "Failed to create cpu dispatcher !\n";
                return 0;
        }
        mSceneDesc.cpuDispatcher = mCpuDispatcher;
        mScene = mPhysics->createScene(mSceneDesc);
        if(!mScene)
        {
                std::cerr << "Failed to create scene !\n";
                return 0;
        }
        
        Cube my_cube(mPhysics, true, glm::vec3(0.0f, 0.0f, 0.0f));
        Cube cube1(mPhysics, false, glm::vec3(0.7f, 3.0f, 0.7f));

        mScene->addActor(*my_cube.mActor);
        mScene->addActor(*cube1.mActor);

        /** Main game loop **/
        while(window->IsRunning())
        {
                float mStepSize = 1.0f/60.0f;
                mScene->simulate(mStepSize);
                mScene->fetchResults(true);

                engine->OnFrameStart();

                glm::mat4 view = glm::mat4(1.0f);
                view = glm::translate(view, glm::vec3(0.0f, -1.0f, -6.0f));
                view = glm::rotate(view, 20.0f, glm::vec3(-1.0f, 0.0f, 0.0f));

                // Build the projection matrix
                glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window->Width()/(float)window->Height(), 0.1f, 100.0f);

                my_cube.Draw(view, projection);
                cube1.Draw(view, projection);
                
                engine->OnFrameEnd();

        }
        mCpuDispatcher->release();
        PxCloseExtensions();
        transport->release();
        mPvd->release();
        mPhysics->release();
        mFoundation->release();

        glUseProgram(0);
        delete window;
        delete renderer;
        delete engine;
}