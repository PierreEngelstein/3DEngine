#pragma once

#include <Common/IEngine.hpp>

namespace Core
{
    class GameEngine : public Common::IEngine
    {
        public:
            GameEngine(Common::IWindow* win);
            ~GameEngine();

            virtual bool Init() override;

            virtual void OnFrameStart() override;

            virtual void Update() override;

            virtual void OnFrameEnd() override;

            virtual void ShutDown() override;

            virtual Common::IWindow* GetWindow() const override;

        
        private:
            Common::IWindow*    m_window;
    };
}