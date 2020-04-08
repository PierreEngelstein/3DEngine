#include <Physics/PhysicsSystem.hpp>
#include <Physics/RigidBodyComponent.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <Core/TransformComponent.hpp>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define NDEBUG
#include "PxPhysicsAPI.h"

namespace Physics
{
    PhysicsSystem::PhysicsSystem()
    {
        std::cout << "New physics system !\n";

        static physx::PxDefaultAllocator gDefaultAllocator;
        static physx::PxDefaultErrorCallback gDefaultErrorCallback;

        m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocator, gDefaultErrorCallback);
        if(!m_foundation)
        {
            std::cerr << "PxFoundation creation failed !\n";
            return;
        }

        m_pvd = physx::PxCreatePvd(*m_foundation);
        m_transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        m_pvd->connect(*m_transport, physx::PxPvdInstrumentationFlag::eALL);
        
        m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), true, m_pvd);
        if(!m_physics)
        {
            std::cerr << "PxCreatePhysics failed !\n";
            return;
        }
        if(!PxInitExtensions(*m_physics, m_pvd))
        {
            std::cerr << "PxInitExtensions failed !\n";
            return;
        }

        // Create a physx scene and assign a correct gravity
        physx::PxSceneDesc mSceneDesc(m_physics->getTolerancesScale());
        mSceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
        mSceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
        m_cpudispatcher = physx::PxDefaultCpuDispatcherCreate(2);
        if(!m_cpudispatcher)
        {
            std::cerr << "Failed to create cpu dispatcher !\n";
            return;
        }
        mSceneDesc.cpuDispatcher = m_cpudispatcher;
        m_scene = m_physics->createScene(mSceneDesc);
        if(!m_scene)
        {
            std::cerr << "Failed to create scene !\n";
            return;
        }
    }
    PhysicsSystem::~PhysicsSystem()
    {
        std::cout << "Goodbye physics\n";

        m_cpudispatcher->release();
        PxCloseExtensions();
        m_transport->release();
        m_pvd->release();
        m_physics->release();
        m_foundation->release();
    }

    void PhysicsSystem::FirstRun()
    {

    }
    
    void PhysicsSystem::Run()
    {
        float mStepSize = 1.0f/60.0f;
        m_scene->simulate(mStepSize);
        m_scene->fetchResults(true);
    }

    void PhysicsSystem::LastRun()
    {

    }

    void PhysicsSystem::CreateBoxRigidBody(EntityID id, bool isStatic)
    {
        ecsengine.GetComponentManager().AddComponent<RigidBodyComponent>(id);
        
        RigidBodyComponent* rb = ecsengine.GetComponentManager().GetComponent<RigidBodyComponent>(id);
        if(rb == nullptr) return;

        Core::TransformComponent* transform = ecsengine.GetComponentManager().GetComponent<Core::TransformComponent>(id);
        if(transform == nullptr) return;

        // Physics material parameters
        physx::PxReal density = 1.0f;
        physx::PxReal staticFriction = 0.5f;
        physx::PxReal dynamicFriction = 0.5f;
        physx::PxReal restitution = 0.5f;
        
        physx::PxVec3 dimensions(0.5f, 0.5f, 0.5f);
        physx::PxBoxGeometry geometry(dimensions);

        physx::PxMaterial *mMaterial = m_physics->createMaterial(staticFriction, dynamicFriction, restitution);
        physx::PxShape *shape = m_physics->createShape(physx::PxBoxGeometry(dimensions), *mMaterial);
        physx::PxTransform m_transform(physx::PxVec3(transform->m_position.x, transform->m_position.y, transform->m_position.z));

        if(isStatic)
        {
            rb->m_actor = PxCreateStatic(*m_physics, m_transform, *shape);
        }
        else
        {
            rb->m_actor = PxCreateDynamic(*m_physics, m_transform, geometry, *mMaterial, density);
            rb->m_actor->attachShape(*shape);
            physx::PxRigidBodyExt::updateMassAndInertia(*(physx::PxRigidDynamic*)rb->m_actor, 10.0f);
        }
        shape->release();
        mMaterial->release();

        m_scene->addActor(*(rb->m_actor));
    }

    void PhysicsSystem::UpdateTransformFromSimulation(EntityID id)
    {
        RigidBodyComponent* rb = ecsengine.GetComponentManager().GetComponent<RigidBodyComponent>(id);
        if(rb == nullptr) return;

        Core::TransformComponent* transform = ecsengine.GetComponentManager().GetComponent<Core::TransformComponent>(id);
        if(transform == nullptr) return;

        physx::PxU32 nbShapes = rb->m_actor->getNbShapes();
        physx::PxShape ** shapes = new physx::PxShape*[nbShapes];
        rb->m_actor->getShapes(shapes, nbShapes);
        while(nbShapes--)
        {
            physx::PxTransform np = physx::PxShapeExt::getGlobalPose(*shapes[nbShapes], *(rb->m_actor));
            transform->m_position.x = np.p.x;
            transform->m_position.z = np.p.y;
            transform->m_position.y = np.p.z;
            glm::qua quat(np.q.x, np.q.y, np.q.z, np.q.w);
            transform->m_rotation = glm::eulerAngles(quat);
        }

        delete[] shapes;
    }
}