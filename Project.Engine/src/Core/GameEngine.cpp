#include <Core/GameEngine.hpp>
#include <cassert>

namespace Core
{
    GameEngine::GameEngine(Common::IWindow* win) : m_window(win)
    {
        assert(m_window != nullptr);
    }
    GameEngine::~GameEngine()
    {

    }

    bool GameEngine::Init()
    {
        return true;
    }

    void GameEngine::OnFrameStart()
    {
    }

    void GameEngine::Update()
    {
    }

    void GameEngine::OnFrameEnd()
    {
        m_window->Update();
    }

    void GameEngine::ShutDown()
    {
    }

    Common::IWindow* GameEngine::GetWindow() const
    {
        return m_window;
    }
}