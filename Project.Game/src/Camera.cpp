
#include "Camera.h"
#include <EntityComponentSystem/ECSEngine.hpp>
#include <EntityComponentSystem/IEntity.hpp>
#include <Core/CameraComponent.hpp>
#include <Core/TransformComponent.hpp>
#include <Core/InputSystem.hpp>
#include <Core/ScriptingSystem.hpp>
#include <Logging/Logger.hpp>

Camera::Camera(glm::vec3 position, float yaw, float pitch, float fov, bool freeCam) : m_freeCam(freeCam) {
    m_id = ecsengine.GetEntityManager().CreateEntity<ECS::IEntity>();
    LOG_DEBUG("Camera has id %d\n", m_id)
    ecsengine.GetComponentManager().AddComponent<Core::CameraComponent>(m_id);
    auto camera = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(m_id);
    assert(camera != nullptr);
    auto *scripting = ecsengine.GetSystemManager().GetSystem<Core::ScriptingSystem>();
    assert(scripting != nullptr);

    camera->Position = position;
    camera->NearPlane = 0.1f;
    camera->FarPlane = 100.0f;
    camera->FieldOfView = fov;
    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->UpVector = glm::vec3(0, 0, 1);

    auto fr = camera->Front();

    LOG_DEBUG("Camera front : %5.2f, %5.2f, %5.2f\n", fr.x, fr.y, fr.z)

    auto input = ecsengine.GetSystemManager().GetSystem<Core::InputSystem>();

    scripting->AddSystem(m_id, [&](EntityID id)
    {
        auto cam = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(id);
        if(cam->yaw > 360) cam->yaw = 0.0f;
    });

    input->Subscribe("forward", m_id, [&](EntityID id)
    {
        auto cam = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(id);
        glm::vec3 front = cam->Front();
        cam->Position += 0.0005f * glm::vec3 (front.x, front.y, m_freeCam ? front.x : 0);
    });
    input->Subscribe("backward", m_id, [&](EntityID id)
    {
        auto cam = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(id);
        glm::vec3 front = cam->Front();
        cam->Position -= 0.0005f * glm::vec3 (front.x, front.y, m_freeCam ? front.x : 0);
    });
    input->Subscribe("left", m_id, [&](EntityID id)
    {
        auto cam = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(id);
        cam->Position -= glm::normalize(glm::cross(cam->Front(), cam->UpVector)) * 0.0005f;
    });
    input->Subscribe("right", m_id, [&](EntityID id)
    {
        auto cam = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(id);
        cam->Position += glm::normalize(glm::cross(cam->Front(), cam->UpVector)) * 0.0005f;
    });
    input->Subscribe("mousemove", m_id, [&](EntityID id)
    {
        auto cam = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(id);
        cam->yaw -= input->DeltaX() * 0.08f;
        cam->pitch -= input->DeltaY() * 0.08f;
    });

}

EntityID Camera::ID() const {
    return m_id;
}
