#include <Core/InputSystem.hpp>
#include <Common/IWindow.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Logging/Logger.hpp>

namespace Core
{
    InputSystem::InputSystem(Common::IWindow* win) : m_win(win)
    {

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
        if(m_win->KeyPressed(GLFW_KEY_E))
        {
            for(auto j : m_map["test"])
            {
                j.second(j.first);
            }
        }
    }

    void InputSystem::LastRun()
    {

    }
}