#pragma once

#include <Types.hpp>
#include <Common/IWindow.hpp>
#include <Common/IRenderer.hpp>

namespace Common
{
    /**
     * @brief Base engine interface.
     * 
     */
    class IEngine
    {
        public:
            virtual ~IEngine(){}
            /** Initializes the engine, returns false if something went wrong  */
            virtual bool Init() = 0;

            /** Handles the work needed at the start of a frame. */
            virtual void OnFrameStart() = 0;

            /** Updates the engine. Should be called for every frame. */
            virtual void Update() = 0;

            /** Handles the work needed at the end of a frame. */
            virtual void OnFrameEnd() = 0;

            /** Shuts down the engine. */
            virtual void ShutDown() = 0;

            /** Gets the current window in use by the engine. */
            virtual IWindow* GetWindow() const = 0;

            /** Gets the current renderer in use by the engine. */
            virtual IRenderer* GetRenderer() const = 0;
    };
}