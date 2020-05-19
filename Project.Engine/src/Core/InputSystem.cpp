#include <Core/InputSystem.hpp>
#include <Core/GLWindow.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Logging/Logger.hpp>

namespace Core
{
    double InputSystem::mousePosX=0, InputSystem::mousePosY=0, InputSystem::mouseDeltaX=0, InputSystem::mouseDeltaY=0;
    bool InputSystem::startFrame = true, InputSystem::doMouseInput = false;
    InputSystem::InputSystem(Common::IWindow* win) : m_win(win), hidden(true)
    {
        m_win->SetCallback(CALLBACK_CURSOR, reinterpret_cast<void*>(mouse_pos_callback));
        if(hidden)
            m_win->HideCursor();
    }

    void InputSystem::Subscribe(const std::string& input, EntityID id, const std::function<void(EntityID id)>& fct)
    {
        if(m_map[input].find(id) != m_map[input].end())
        {
            LOG(InputSystem, Logging::Warning, "Entity %d already registered input %s!\n", id, input.c_str());
            return;
        }

        m_map[input][id] = fct;
    }

    void InputSystem::FirstRun()
    {

    }

    void InputSystem::Run()
    {
        if(m_win->KeyPressed(GLFW_KEY_ESCAPE))
            for(auto j : m_map["escape"]) j.second(j.first);
        if(m_win->KeyPressed(GLFW_KEY_W))
            for(auto j : m_map["forward"]) j.second(j.first);
        if(m_win->KeyPressed(GLFW_KEY_S))
            for(auto j : m_map["backward"]) j.second(j.first);
        if(m_win->KeyPressed(GLFW_KEY_A))
            for(auto j : m_map["left"]) j.second(j.first);
        if(m_win->KeyPressed(GLFW_KEY_D))
            for(auto j : m_map["right"]) j.second(j.first);
        if(doMouseInput)
        {
            for(auto j : m_map["mousemove"]) j.second(j.first);
            doMouseInput = false;
        }

    }

    void InputSystem::LastRun()
    {

    }

    void InputSystem::mouse_pos_callback(GLFWwindow *win, double xPos, double yPos)
    {
        if(startFrame)
        {
            mousePosX = xPos;
            mousePosY = yPos;
            startFrame = false;
        }
        mouseDeltaX = xPos - mousePosX;
        mouseDeltaY = yPos - mousePosY;
        mousePosX = xPos;
        mousePosY = yPos;
        doMouseInput = true;

    }

    double InputSystem::DeltaX() {
        return mouseDeltaX;
    }

    double InputSystem::DeltaY() {
        return mouseDeltaY;
    }

    void InputSystem::ShowCursor() {
        m_win->ShowCursor();
        hidden = false;
    }

    void InputSystem::HideCursor() {
        m_win->HideCursor();
        hidden = true;
    }

    bool InputSystem::CursorState() const {
        return hidden;
    }
}