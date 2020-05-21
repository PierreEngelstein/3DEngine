#ifndef INC_3DENGINE_CAMERA_H
#define INC_3DENGINE_CAMERA_H

#include "Types.hpp"
#include "glm/glm.hpp"

class Camera
{
    public:
        Camera(glm::vec3 position, float yaw, float pitch, float fov, bool freeCam = true);

        EntityID ID() const;
    private:
        EntityID m_id;
        bool m_freeCam;
};


#endif //INC_3DENGINE_CAMERA_H
