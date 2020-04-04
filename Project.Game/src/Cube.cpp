#include "Cube.hpp"
#include "GL/glew.h"
#include <string>
#include <iostream>
#include <fstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"

using namespace physx;

int LoadFile_1(std::string path, std::string *text)
{
        std::ifstream file(path.c_str(), std::ios::in);
        if(file.is_open())
        {
                std::string text_tmp;
                file.seekg(0, std::ios::end);
                text_tmp.resize(file.tellg());
                file.seekg(0, std::ios::beg);
                file.read(&text_tmp[0], text_tmp.size());
                file.close();
                *text = text_tmp;
                return 0;
        }
        return 1;
}

Cube::Cube(PxPhysics *mPhysics, bool isStatic, glm::vec3 pos) : position(pos)
{
        // Compile vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        std::string vertex_text;
        if(LoadFile_1("resources/vertex.shad", &vertex_text))
        {
                std::cerr << "Failed to load vertex shader !\n";
                return;
        }
        const char *str = vertex_text.c_str();
        glShaderSource(vertexShader, 1, &str, NULL);
        glCompileShader(vertexShader);
        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
                GLint maxLength = 0;
                glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
                std::vector<GLchar> errorLog(maxLength);
                glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);
                fprintf(stderr, "%s\n", errorLog.data());
                return;
        }

        //Compile fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        if(LoadFile_1("resources/frag.shad", &vertex_text))
        {
                std::cerr << "Failed to load fragment shader !\n";
                return;
        }
        str = vertex_text.c_str();
        glShaderSource(fragmentShader, 1, &str, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
                GLint maxLength = 0;
                glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
                std::vector<GLchar> errorLog(maxLength);
                glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);
                fprintf(stderr, "%s\n", errorLog.data());
                return;
        }

        //Link fragment & vertex
        shader = glCreateProgram();
        glAttachShader(shader, vertexShader);
        glAttachShader(shader, fragmentShader);
        glLinkProgram(shader);
        isCompiled = 0;
        glGetProgramiv(shader, GL_LINK_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
                GLint maxLength = 0;
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                std::vector<GLchar> errorLog(maxLength);
                glGetProgramInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
                fprintf(stderr, "%s\n", errorLog.data());
                return;
        }

        //Validate the shader
        glValidateProgram(shader);
        isCompiled = 0;
        glGetProgramiv(shader, GL_VALIDATE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
                GLint maxLength = 0;
                glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                std::vector<GLchar> errorLog(maxLength);
                glGetProgramInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
                fprintf(stderr, "%s\n", errorLog.data());
                return;
        }

        //Clear
        glDetachShader(shader, vertexShader);
        glDetachShader(shader, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        modelUniform = glGetUniformLocation(shader, "model");
        projectionUniform = glGetUniformLocation(shader, "projection");
        textureUniform = glGetUniformLocation(shader, "texture_sampler");
        viewUniform = glGetUniformLocation(shader, "view");

        // Generate a vao
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        // Build the vertex vbo
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, Cube::verticesSquare.size() * sizeof(glm::vec3), &Cube::verticesSquare[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        // Build the texture vbo
        glGenBuffers(1, &textureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
        glBufferData(GL_ARRAY_BUFFER, Cube::textCoordSquare.size() * sizeof(glm::vec2), &Cube::textCoordSquare[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
        // Build the index vbo
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Cube::indicesSquare.size() * sizeof(glm::vec2), &Cube::indicesSquare[0], GL_STATIC_DRAW);
        glBindVertexArray(0);

        // Create the physics
        PxReal density = 1.0f;
        PxVec3 dimensions(0.5f, 0.5f, 0.5f);
        PxBoxGeometry geometry(dimensions);

        PxMaterial *mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
        PxShape *shape = mPhysics->createShape(PxBoxGeometry(dimensions), *mMaterial);
        PxTransform m_transform(PxVec3(position.x, position.y, position.z));

        std::cout << position.x << "," << position.y << "," << position.z << "\n";
        
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
        glDeleteProgram(shader);
}

void Cube::Draw(glm::mat4 view, glm::mat4 projection)
{
        PxU32 nbShapes = mActor->getNbShapes();
        PxShape ** shapes = new PxShape*[nbShapes];
        mActor->getShapes(shapes, nbShapes);
        while(nbShapes--)
        {
                PxTransform np = PxShapeExt::getGlobalPose(*shapes[nbShapes], *mActor);
                position.x = np.p.x;
                position.z = np.p.y;
                position.y = np.p.z;
                glm::qua quat(np.q.x, np.q.y, np.q.z, np.q.w);
                angle = glm::eulerAngles(quat);

        }
        glUseProgram(shader);
        glm::mat4 model = glm::identity<glm::mat4>();
        model = glm::translate(model, position);
        model = glm::rotate(model, angle.z, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, angle.x, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, angle.y, glm::vec3(0.0f, 0.0f, 1.0f));
        
        
        // model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

        glUniform1i(textureUniform, 0);
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        // Bind the vao for use in shader
        glBindVertexArray(vao);
        // Enables the 2 first elements in the vertex attributes array
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        // Draw what is needed
        glDrawElements(GL_TRIANGLES, Cube::indicesSquare.size(), GL_UNSIGNED_INT, (void *)0);
        // Disables the elements in the vertex attributes array for the next draw to be clean
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        // Unbind the vao from the shader
        glBindVertexArray(0);
        glUseProgram(0);
}



std::vector<glm::vec3> Cube::verticesSquare =
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

std::vector<glm::vec2> Cube::textCoordSquare =
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

std::vector<unsigned int> Cube::indicesSquare =
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