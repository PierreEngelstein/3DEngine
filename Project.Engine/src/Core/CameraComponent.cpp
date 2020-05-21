#include <Core/CameraComponent.hpp>

glm::vec3 Core::CameraComponent::Front()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.z = sin(glm::radians(pitch));
    return glm::normalize(direction);
}
