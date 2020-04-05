#pragma once

#include <EntityComponentSystem/IComponent.hpp>
#include "glm/glm.hpp"
#include <vector>

namespace Graphics
{
    class Mesh3DComponent : public ECS::IComponent
    {
        public:
            Mesh3DComponent() : m_vertices(std::vector<glm::vec3>(0)), m_uv(std::vector<glm::vec2>(0)), m_triangles(std::vector<unsigned int>(0)){}
            Mesh3DComponent(
                std::vector<glm::vec3> vert,
                std::vector<glm::vec2> uv,
                std::vector<unsigned int> ind
            ): m_vertices(vert), m_uv(uv), m_triangles(ind){}

            ~Mesh3DComponent()
            {
                m_vertices.clear();
                m_uv.clear();
                m_triangles.clear();
            }

            std::vector<glm::vec3>    m_vertices;
            std::vector<glm::vec2>    m_uv;
            std::vector<unsigned int> m_triangles;
            unsigned int              m_vao;
            unsigned int              m_vertexBuffer;
            unsigned int              m_textureBuffer;
            unsigned int              m_indexBuffer;
    };
}