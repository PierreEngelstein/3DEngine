#pragma once

#include <Common/IRenderer.hpp>

namespace Graphics
{
    class GLRenderer : public Common::IRenderer
    {
        public:
            GLRenderer();
            ~GLRenderer();
            virtual bool Init(unsigned int width, unsigned int height) override;

            virtual void AddDebugMesh() override;

            virtual void OnFrameStart() override;

            virtual void Render() override;

            virtual void DrawDebug() override;

            virtual void OnFrameEnd() override;

            virtual void SetViewport(unsigned int width, unsigned int height) override;

            virtual void ShutDown() override;
        private:
            unsigned int m_width, m_height;
    };
}