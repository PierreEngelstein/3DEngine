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

using namespace physx;

Cube::Cube(PxPhysics *mPhysics, bool isStatic, glm::vec3 pos)
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

        // Create the physics
        PxReal density = 1.0f;
        PxVec3 dimensions(0.5f, 0.5f, 0.5f);
        PxBoxGeometry geometry(dimensions);

        PxMaterial *mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
        PxShape *shape = mPhysics->createShape(PxBoxGeometry(dimensions), *mMaterial);
        PxTransform m_transform(PxVec3(transform->m_position.x, transform->m_position.y, transform->m_position.z));

        std::cout << transform->m_position.x << "," << transform->m_position.y << "," << transform->m_position.z << "\n";
        
        if(isStatic)
        {
                mActor = PxCreateStatic(*mPhysics, m_transform, *shape);
        }
        else
        {
                mActor = PxCreateDynamic(*mPhysics, m_transform, geometry, *mMaterial, density);
                mActor->attachShape(*shape);
                PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic*)mActor, 10.0f);
        }
        shape->release();
        mMaterial->release();
}

Cube::~Cube()
{
        // delete m_material;
        // delete m_mesh;
}

void Cube::Draw(glm::mat4 view, glm::mat4 projection)
{
        Core::TransformComponent* transform = ecsengine.GetComponentManager().GetComponent<Core::TransformComponent>(m_id);

        PxU32 nbShapes = mActor->getNbShapes();
        PxShape ** shapes = new PxShape*[nbShapes];
        mActor->getShapes(shapes, nbShapes);
        while(nbShapes--)
        {
                PxTransform np = PxShapeExt::getGlobalPose(*shapes[nbShapes], *mActor);
                transform->m_position.x = np.p.x;
                transform->m_position.z = np.p.y;
                transform->m_position.y = np.p.z;
                glm::qua quat(np.q.x, np.q.y, np.q.z, np.q.w);
                transform->m_rotation = glm::eulerAngles(quat);
        }

        delete[] shapes;

        Graphics::GraphicsSystem* system = ecsengine.GetSystemManager().GetSystem<Graphics::GraphicsSystem>();
        assert(system != nullptr);
        system->DrawEntity(m_id, view, projection);

        // Graphics::MaterialComponent* material = ecsengine.GetComponentManager().GetComponent<Graphics::MaterialComponent>(m_id);
        // Graphics::Mesh3DComponent* mesh = ecsengine.GetComponentManager().GetComponent<Graphics::Mesh3DComponent>(m_id);

        // material->m_shader->Use();
        // material->m_texture->Use();

        // glm::mat4 model = glm::identity<glm::mat4>();
        // model = glm::translate(model, position);
        // model = glm::rotate(model, angle.z, glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, angle.x, glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::rotate(model, angle.y, glm::vec3(0.0f, 0.0f, 1.0f));
        // // model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

        // material->m_shader->AssignUniform("texture_sampler", 0);
        // material->m_shader->AssignUniform("model", model);
        // material->m_shader->AssignUniform("projection", projection);
        // material->m_shader->AssignUniform("view", view);

        // // Bind the vao for use in shader
        // glBindVertexArray(mesh->m_vao);
        // // Enables the 2 first elements in the vertex attributes array
        // glEnableVertexAttribArray(0);
        // glEnableVertexAttribArray(1);
        // // Draw what is needed
        // glDrawElements(GL_TRIANGLES, Cube::indicesSquare.size(), GL_UNSIGNED_INT, (void *)0);
        // // Disables the elements in the vertex attributes array for the next draw to be clean
        // glDisableVertexAttribArray(0);
        // glDisableVertexAttribArray(1);
        // // Unbind the vao from the shader
        // glBindVertexArray(0);
        // material->m_shader->Stop();
}