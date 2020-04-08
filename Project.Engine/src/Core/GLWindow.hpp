#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <Common/IWindow.hpp>


#define CALLBACK_CHARACTER 0
#define CALLBACK_KEY       1
#define CALLBACK_CURSOR    2

class GLFWwindow;
namespace Core
{
        /**
         * @brief IWindow implementation for OpenGL
         * 
         */
        class GLWindow : public Common::IWindow
        {
                public:
                        GLWindow(int width, int height, std::string name);
                        ~GLWindow();

                        void Update() override;
                        bool IsRunning() override;
                        int Width() const override;
                        int Height() const override;

                        int& WidthRef() override;
                        int& HeightRef() override;

                        bool KeyPressed(int key) const override;
                        bool KeyReleased(int key) const override;

                        bool MousePressed(int key) const override;
                        bool MouseReleased(int key) const override;

                        void SetCallback(CallbackType type, void *callback) override;
                private:
                        int width, height;
                        std::string name;
                        GLFWwindow *window;

                        static void error_callback(int error, const char *description);
                        static void window_size_callback(GLFWwindow* window, int width, int height);
                        static void drop_callback(GLFWwindow* window, int count, const char** paths); 
        };
}

#endif