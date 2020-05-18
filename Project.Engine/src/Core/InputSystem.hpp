#pragma once

#include <EntityComponentSystem/ISystem.hpp>
#include <string>
#include <functional>
#include <map>
#include <vector>

namespace Common
{
    class IWindow;
}
namespace Core
{
    class InputSystem : public ECS::ISystem
    {
        public:
            InputSystem(Common::IWindow* win);
            void Subscribe(const std::string&  input, EntityID id, const std::function<void(EntityID id)>& fct);

            virtual void FirstRun() override;
            virtual void Run() override;
            virtual void LastRun() override;

            bool KeyPressed(const std::string& name);
            bool KeyReleased(const std::string& name);

            bool GetAxis(const std::string& name);

            static double DeltaX();
            static double DeltaY();

            void HideCursor();
            void ShowCursor();

            bool CursorState() const;

        private:
            std::map<std::string, std::map<EntityID, std::function<void(EntityID id)>>> m_map;
            Common::IWindow* m_win;
            bool hidden;
            static double mousePosX, mousePosY;
            static double mouseDeltaX, mouseDeltaY;
            static bool startFrame, doMouseInput;
            static void mouse_pos_callback(class GLFWwindow* win, double xPos, double yPos);
    };
}
