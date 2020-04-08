#include "Cube.hpp"
#include "GL/glew.h"
#include <string>
#include <iostream>
#include <fstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"
#include "Graphics/GLShader.hpp"
#include <Graphics/Texture.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Graphics/GraphicSystem.hpp>
#include <EntityComponentSystem/ECSEngine.hpp>
#include <EntityComponentSystem/IEntity.hpp>
#include <Core/TransformComponent.hpp>
#include <Core/ScriptingSystem.hpp>

#include <Physics/PhysicsSystem.hpp>

#include <Physics/RigidBodyComponent.hpp>

using namespace physx;

Cube::Cube(bool isStatic, glm::vec3 pos)
{
	m_id = ecsengine.GetEntityManager().CreateEntity<ECS::IEntity>();
	ecsengine.GetComponentManager().AddComponent<Graphics::MaterialComponent>(m_id);
	ecsengine.GetComponentManager().AddComponent<Graphics::Mesh3DComponent>(m_id);
	ecsengine.GetComponentManager().AddComponent<Core::TransformComponent>(m_id);

	Core::TransformComponent* transform = ecsengine.GetComponentManager().GetComponent<Core::TransformComponent>(m_id);
	assert(transform != nullptr);
	transform->m_position = pos;

	Graphics::MaterialComponent* material = ecsengine.GetComponentManager().GetComponent<Graphics::MaterialComponent>(m_id);
	assert(material != nullptr);

	material->m_shader = new Graphics::GLShader();
	if(!material->m_shader->AddProgram("resources/vertex.shad", Common::VERTEX))
	{
		std::cerr << "Failed to load vertex shader.\n";
		return;
	}

	if(!material->m_shader->AddProgram("resources/frag.shad", Common::FRAGMENT))
	{
		std::cerr << "Failed to load vertex shader.\n";
		return;
	}

	material->m_shader->GenerateShader();
	material->m_shader->GenerateUniforms(std::vector<std::string>{"model", "projection", "texture_sampler", "view"});

	material->m_texture = new Graphics::Texture();
	material->m_texture->Load("resources/Untitled.png");

	Graphics::Mesh3DComponent* mesh = ecsengine.GetComponentManager().GetComponent<Graphics::Mesh3DComponent>(m_id);

	Graphics::GraphicsSystem* system = ecsengine.GetSystemManager().GetSystem<Graphics::GraphicsSystem>();
	assert(system != nullptr);
	system->GenerateVertexArray(m_id);

	Physics::PhysicsSystem *phys = ecsengine.GetSystemManager().GetSystem<Physics::PhysicsSystem>();
	assert(phys != nullptr);
	phys->CreateBoxRigidBody(m_id, isStatic);

}

Cube::~Cube()
{
}