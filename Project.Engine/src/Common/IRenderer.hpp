#pragma once

namespace Common
{
    /**
     * @brief Base renderer interface. 
     * 
     */
    class IRenderer
    {
        public:
            virtual ~IRenderer(){}
            /**
             * @brief Initializes the renderer.
             * 
             * @param width : the width of the viewport.
             * @param height : the height of the viewport.
             * @return true if no problem occured.
             * @return false otherwise.
             */
            virtual bool Init(unsigned int width, unsigned int height) = 0;

            /** Adds a debug mesh to the renderer. */
            virtual void AddDebugMesh() = 0;

            /** Handles work needed at start of the frame. */
            virtual void OnFrameStart() = 0;

            /** Handles work needed during render time. */
            virtual void Render() = 0;

            /** Handle debug drawing. */
            virtual void DrawDebug() = 0;

            /** Handles work needed at the end of the frame. */
            virtual void OnFrameEnd() = 0;

            /** Updates the viewport. */
            virtual void SetViewport(unsigned int width, unsigned int height) = 0;

            /** Shutdowns the renderer. */
            virtual void ShutDown() = 0;
    };
}