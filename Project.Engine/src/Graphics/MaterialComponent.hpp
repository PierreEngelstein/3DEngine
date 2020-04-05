#pragma once

#include "glm/glm.hpp"

#include <EntityComponentSystem/IComponent.hpp>
#include <Common/IShader.hpp>
#include <Common/ITexture.hpp>
#include <iostream>

namespace Graphics
{
    class MaterialComponent : public ECS::IComponent
    {
        public:
            ~MaterialComponent()
            {
                std::cout << "Cleared materialcomponent!\n";
                delete m_shader;
                delete m_texture;
            }
            /** The shader used by the material. */
            Common::IShader*  m_shader;
            /** The texture used by the material. */
            Common::ITexture* m_texture;
            /** The color used by the material. */
            glm::vec3         m_color;
    };
}