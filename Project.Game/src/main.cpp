#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Cube.hpp"
#include "lodepng.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define NDEBUG
#include "PxPhysicsAPI.h"

int win_width = 800, win_height = 600;
using namespace physx;

void error_callback(int error, const char *desc)
{
        std::cerr << "GLFW error : " << desc << "\n";
        return;
}
void resizeCallback(GLFWwindow *win, int _width, int _height)
{
        win_width = _width;
        win_height = _height;
}

int main()
{
#pragma region GLFW Initialisation
        std::cout << "Hello World\n";
        if(!glfwInit())
        {
                std::cerr << "Failed to init GLFW\n";
                return -1;
        }
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

        GLFWwindow *window = glfwCreateWindow(win_width, win_height, "Window", nullptr, nullptr);
        glfwSetErrorCallback(error_callback);
        glfwSetWindowSizeCallback(window, resizeCallback);

        glfwMakeContextCurrent(window);
        glfwShowWindow(window);
        glfwSwapInterval(1);
#pragma endregion

#pragma region OpenGL & GLEW init
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(glewInit() != GLEW_OK)
        {
                std::cerr << "Failed to init glew !\n";
                return -1;
        }
        if(!GLEW_VERSION_3_0)
        {
                std::cerr << "No correct glew version!\n";
                return -1;
        }
#pragma endregion

#pragma region Texture
        std::vector<unsigned char> data;
        unsigned width, height;
        unsigned error = lodepng::decode(data, width, height, "resources/Untitled.png");
        if(error)
        {
                std::cerr << "Failed to open image file Untitled.png: " << lodepng_error_text(error);
                return -1;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#pragma endregion

        // Setup PhysX
        static physx::PxDefaultAllocator gDefaultAllocator;
        static PxDefaultErrorCallback gDefaultErrorCallback;
        physx::PxFoundation *mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocator, gDefaultErrorCallback);
        if(!mFoundation)
        {
                std::cerr << "PxFoundation creation failed !\n";
                return 0;
        }

        PxPvd* mPvd = physx::PxCreatePvd(*mFoundation);
        PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
        
        PxPhysics* mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true, mPvd);
        if(!mPhysics)
        {
                std::cerr << "PxCreatePhysics failed !\n";
                return 0;
        }
        if(!PxInitExtensions(*mPhysics, mPvd))
        {
                std::cerr << "PxInitExtensions failed !\n";
                return 0;
        }

        // Create a physx scene and assign a correct gravity
        PxScene *mScene;
        PxSceneDesc mSceneDesc(mPhysics->getTolerancesScale());
        mSceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        mSceneDesc.filterShader = PxDefaultSimulationFilterShader;
        PxDefaultCpuDispatcher *mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
        if(!mCpuDispatcher)
        {
                std::cerr << "Failed to create cpu dispatcher !\n";
                return 0;
        }
        mSceneDesc.cpuDispatcher = mCpuDispatcher;
        mScene = mPhysics->createScene(mSceneDesc);
        if(!mScene)
        {
                std::cerr << "Failed to create scene !\n";
                return 0;
        }
        
        Cube my_cube(mPhysics, true, glm::vec3(0.0f, 0.0f, 0.0f));
        Cube cube1(mPhysics, false, glm::vec3(0.7f, 3.0f, 0.7f));

        mScene->addActor(*my_cube.mActor);
        mScene->addActor(*cube1.mActor);

        /** Main game loop **/
        while(!glfwWindowShouldClose(window))
        {
                float mStepSize = 1.0f/60.0f;
                mScene->simulate(mStepSize);
                mScene->fetchResults(true);

                // Update OpenGL
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glClearColor(0.17578125f, 0.17578125f, 0.17578125f, 0.0f);
                glViewport(0, 0, win_width, win_height);
                // Tell OpenGL to use this texture
                glEnable(GL_TEXTURE_2D);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture);

                glm::mat4 view = glm::mat4(1.0f);
                view = glm::translate(view, glm::vec3(0.0f, -1.0f, -6.0f));
                view = glm::rotate(view, 20.0f, glm::vec3(-1.0f, 0.0f, 0.0f));

                // Build the projection matrix
                glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)win_width/(float)win_height, 0.1f, 100.0f);

                my_cube.Draw(view, projection);
                cube1.Draw(view, projection);

                // Updates everything needed in the window
                glfwSwapBuffers(window);
                glfwPollEvents();
        }
        PxCloseExtensions();
        transport->release();
        mPvd->release();
        mPhysics->release();
        mFoundation->release();

#pragma region GLFW Clear
        glUseProgram(0);
        glfwDestroyWindow(window);
        glfwTerminate();
#pragma endregion
}