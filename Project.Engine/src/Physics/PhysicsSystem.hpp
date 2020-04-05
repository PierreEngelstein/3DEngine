#pragma once

#include <EntityComponentSystem/ISystem.hpp>

// Forward-declare every physx class here if needed by the system.
namespace physx
{
    class PxScene;
    class PxPhysics;
    class PxDefaultCpuDispatcher;
    class PxPvdTransport;
    class PxPvd;
    class PxFoundation;
}
namespace Physics
{
    /**
     * @brief The physics system.
     * 
     */
    class PhysicsSystem : public ECS::ISystem
    {

        public:
            PhysicsSystem();
            ~PhysicsSystem();

            virtual void FirstRun() override;
            virtual void Run() override;
            virtual void LastRun() override;

            void CreateBoxRigidBody(EntityID id, bool isStatic);
            void UpdateTransformFromSimulation(EntityID id);
            
        private:
            physx::PxScene* m_scene;
            physx::PxPhysics* m_physics;
            physx::PxDefaultCpuDispatcher* m_cpudispatcher;
            physx::PxPvdTransport* m_transport;
            physx::PxPvd* m_pvd;
            physx::PxFoundation* m_foundation;
    };
    
    
}