#include <Graphics/GraphicSystem.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/MaterialComponent.hpp>
#include <Core/TransformComponent.hpp>
#include <Core/CameraComponent.hpp>
#include <Common/IWindow.hpp>
#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

std::vector<glm::vec3> Graphics::GraphicsSystem::verticesSquare =
{
        glm::vec3(-0.5f,0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,0.5f,-0.5f),
 
        glm::vec3(-0.5f,0.5f,0.5f),
        glm::vec3(-0.5f,-0.5f,0.5f),
        glm::vec3(0.5f,-0.5f,0.5f),
        glm::vec3(0.5f,0.5f,0.5f),
 
        glm::vec3(0.5f,0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,0.5f),
        glm::vec3(0.5f,0.5f,0.5f),
 
        glm::vec3(-0.5f,0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,0.5f),
        glm::vec3(-0.5f,0.5f,0.5f),
 
        glm::vec3(-0.5f,0.5f,0.5f),
        glm::vec3(-0.5f,0.5f,-0.5f),
        glm::vec3(0.5f,0.5f,-0.5f),
        glm::vec3(0.5f,0.5f,0.5f),
 
        glm::vec3(-0.5f,-0.5f,0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f,-0.5f,0.5f)
};

std::vector<glm::vec2> Graphics::GraphicsSystem::textCoordSquare =
{
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,1),
        glm::vec2(1,0),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,1),
        glm::vec2(1,0),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,1),
        glm::vec2(1,0),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,1),
        glm::vec2(1,0),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,1),
        glm::vec2(1,0),
        glm::vec2(0,0),
        glm::vec2(0,1),
        glm::vec2(1,1),
        glm::vec2(1,0)
};

std::vector<unsigned int> Graphics::GraphicsSystem::indicesSquare =
{
        0,1,3,
        3,1,2,
        4,5,7,
        7,5,6,
        8,9,11,
        11,9,10,
        12,13,15,
        15,13,14,
        16,17,19,
        19,17,18,
        20,21,23,
        23,21,22
};
namespace Graphics
{
	GraphicsSystem::GraphicsSystem(Common::IWindow* win) : m_win(win)
	{
		glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(glewInit() != GLEW_OK)
        {
			std::cerr << "Failed to init glew !\n";
			return;
        }
        if(!GLEW_VERSION_3_0)
        {
			std::cerr << "No correct glew version!\n";
			return;
        }
	}

    void GraphicsSystem::FirstRun()
    {

    }

    void GraphicsSystem::Run()
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.17578125f, 0.17578125f, 0.17578125f, 0.0f);
        glViewport(0, 0, m_win->Width(), m_win->Height());
		
        Graphics::GraphicsSystem *graphics = ecsengine.GetSystemManager().GetSystem<Graphics::GraphicsSystem>();
		if(graphics == nullptr) return;

		auto tr = ecsengine.GetComponentManager().GetComponent<Core::CameraComponent>(GetCameraID());

		glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(tr->Position, tr->Position + tr->Front(), tr->UpVector);
		glm::mat4 projection = glm::perspective(glm::radians(tr->FieldOfView), (float)m_win->Width()/(float)m_win->Height(), tr->NearPlane, tr->FarPlane);

        ecsengine.GetEntityManager().Foreach([&](EntityID id)
        {
            graphics->DrawEntity(id, view, projection);
        });
    }

    void GraphicsSystem::LastRun()
    {

    }

    void GraphicsSystem::GenerateVertexArray(EntityID id)
    {
        Mesh3DComponent* component = ecsengine.GetComponentManager().GetComponent<Mesh3DComponent>(id);
        if(component == nullptr) return;

        for(auto i : indicesSquare)
                component->m_triangles.push_back(i);
        for(auto i : textCoordSquare)
                component->m_uv.push_back(i);
        for(auto i : verticesSquare)
                component->m_vertices.push_back(i);

        // Generate a vao
        glGenVertexArrays(1, &component->m_vao);
        glBindVertexArray(component->m_vao);
        // Build the vertex vbo
        glGenBuffers(1, &component->m_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, component->m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, component->m_vertices.size() * sizeof(glm::vec3), &component->m_vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        // Build the texture vbo
        glGenBuffers(1, &component->m_textureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, component->m_textureBuffer);
        glBufferData(GL_ARRAY_BUFFER, component->m_uv.size() * sizeof(glm::vec2), &component->m_uv[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        // Build the index vbo
        glGenBuffers(1, &component->m_indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component->m_indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, component->m_triangles.size() * sizeof(glm::vec2), &component->m_triangles[0], GL_STATIC_DRAW);
        glBindVertexArray(0);
    }

    void GraphicsSystem::DrawEntity(EntityID id, glm::mat4 view, glm::mat4 projection)
    {
        Graphics::MaterialComponent* material = ecsengine.GetComponentManager().GetComponent<Graphics::MaterialComponent>(id);
        Graphics::Mesh3DComponent* mesh = ecsengine.GetComponentManager().GetComponent<Graphics::Mesh3DComponent>(id);
        Core::TransformComponent* transform = ecsengine.GetComponentManager().GetComponent<Core::TransformComponent>(id);

        if(material == nullptr || mesh == nullptr || transform == nullptr) return;

        material->m_shader->Use();
        material->m_texture->Use();

        glm::mat4 model = glm::identity<glm::mat4>();
        model = glm::translate(model, transform->m_position);
        model = glm::rotate(model, transform->m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, transform->m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, transform->m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

        material->m_shader->AssignUniform("texture_sampler", 0);
        material->m_shader->AssignUniform("model", model);
        material->m_shader->AssignUniform("projection", projection);
        material->m_shader->AssignUniform("view", view);

        glBindVertexArray(mesh->m_vao);
        // Enables the 2 first elements in the vertex attributes array
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        // Draw what is needed
        glDrawElements(GL_TRIANGLES, mesh->m_triangles.size() , GL_UNSIGNED_INT, (void *)0);
        // Disables the elements in the vertex attributes array for the next draw to be clean
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        // Unbind the vao from the shader
        glBindVertexArray(0);
        material->m_shader->Stop();
    }
}