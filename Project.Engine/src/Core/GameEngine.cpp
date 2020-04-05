#include <Core/GameEngine.hpp>
#include <cassert>

namespace Core
{

    GameEngine::GameEngine(Common::IWindow* win, Common::IRenderer* rend) : m_window(win), m_renderer(rend)
    {
        assert(m_window != nullptr);
        assert(m_renderer != nullptr);
    }
    GameEngine::~GameEngine()
    {

    }

    bool GameEngine::Init()
    {
        if(!m_renderer->Init(m_window->Width(), m_window->Height()))
            return false;
        return true;
    }

    void GameEngine::OnFrameStart()
    {
        m_renderer->OnFrameStart();
    }

    void GameEngine::Update()
    {
        m_renderer->SetViewport(m_window->Width(), m_window->Height());
        m_renderer->Render();
    }

    void GameEngine::OnFrameEnd()
    {
        m_renderer->OnFrameEnd();

        m_window->Update();
    }

    void GameEngine::ShutDown()
    {
        m_renderer->ShutDown();
    }

    Common::IWindow* GameEngine::GetWindow() const
    {
        return m_window;
    }

    Common::IRenderer* GameEngine::GetRenderer() const
    {
        return m_renderer;
    }
}