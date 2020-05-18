#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLWindow.hpp"
#include <iostream>
#include <Logging/Logger.hpp>

namespace Core
{
        GLWindow::GLWindow(int width, int height, std::string name) : IWindow(width, height, name)
        {
                this->width  = width;
                this->height = height;
                this->name   = name;

                if(!glfwInit())
                {
                        LOG(GLWindow, Logging::Error, "Failed to initialize glfw !\n");
                        return;
                }

                glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
                // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

                this->window = glfwCreateWindow(this->width, this->height, this->name.c_str(), nullptr, nullptr);

                glfwSetErrorCallback(error_callback);

                glfwSetWindowSizeCallback(window, window_size_callback);
                glfwSetDropCallback(window, drop_callback);

                glfwMakeContextCurrent(window);
                glfwShowWindow(window);
                glfwSwapInterval(0);

                LOG(GLWindow, Logging::Info, "Initialized window using %s\n", glfwGetVersionString());
        }

        GLWindow::~GLWindow()
        {
                LOG(GLWindow, Logging::Info, "Closed window.\n");
                glfwDestroyWindow(window);
                glfwTerminate();
        }

        void GLWindow::Update()
        {
                glfwSwapBuffers(window);
                glfwPollEvents();
                glfwGetWindowSize(window, &width, &height);
        }

        bool GLWindow::IsRunning()
        {
                return !glfwWindowShouldClose(window);
        }

        int GLWindow::Width() const
        {
                return this->width;
        }

        int GLWindow::Height() const
        {
                return this->height;
        }

        int& GLWindow::WidthRef()
        {
                return width;
        }
        int& GLWindow::HeightRef()
        {
                return height;
        }

        bool GLWindow::KeyPressed(int key) const
        {
                return glfwGetKey(this->window, key) == GLFW_PRESS;
        }

        bool GLWindow::KeyReleased(int key) const
        {
                return glfwGetKey(this->window, key) == GLFW_RELEASE;
        }

        bool GLWindow::MousePressed(int key) const
        {
                glfwGetMouseButton(this->window, key);
                return glfwGetMouseButton(this->window, key) == GLFW_PRESS;
        }

        bool GLWindow::MouseReleased(int key) const
        {
                return glfwGetMouseButton(this->window, key) == GLFW_RELEASE;
        }
        
        void GLWindow::error_callback(int error, const char *description)
        {
                LOG(GLWindow, Logging::Error, "glfw error : %s\n", description);
                std::cerr << "glfw error : " << description << "\n";
                return;
        }

        void GLWindow::window_size_callback(GLFWwindow* window, int width, int height)
        {
                glfwSwapBuffers(window);
        }

        void GLWindow::drop_callback(GLFWwindow* window, int count, const char** paths)
        {
        }

        void GLWindow::SetCallback(CallbackType type, void *callback)
        {
                switch(type)
                {
                        case CALLBACK_CHARACTER:
                                glfwSetCharCallback(this->window, (GLFWcharfun)callback);
                                LOG(GLWindow, Logging::Info, "Added character callback\n");
                                break;
                        case CALLBACK_KEY:
                                glfwSetKeyCallback(this->window, (GLFWkeyfun)callback);
                                LOG(GLWindow, Logging::Info, "Added key callback\n");
                                break;
                        case CALLBACK_CURSOR:
                                glfwSetCursorPosCallback(this->window, (GLFWcursorposfun)callback);
                                LOG(GLWindow, Logging::Info, "Added cursor callback\n");
                                break;
                        default:
                                break;
                }
        }

        void GLWindow::ShowCursor() {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        void GLWindow::HideCursor() {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
}