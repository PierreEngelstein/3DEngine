#pragma once

#include <string>
#include <Types.hpp>

namespace Common
{
	/**
	 * @brief Base interface to expose windowing functionalities to the engine.
	 * 
	 */
	class IWindow
	{
		public:
			/**
			 * @brief Creates a new window given width, height and name.
			 * 
			 */
			IWindow(int width, int height, std::string name){};

			virtual ~IWindow(){}

			/**
			 * @brief Updates the window.
			 * 
			 */
			virtual void Update() = 0;

			/**
			 * @brief Checks if the window is running or not.
			 * 
			 * @return true if it is running.
			 * @return false otherwise.
			 */
			virtual bool IsRunning() = 0;

			/**
			 * @brief Gets the current window width.
			 * 
			 */
			virtual int Width() const = 0;

			virtual int& WidthRef() = 0;

			/**
			 * @brief Gets the current window height.
			 * 
			 */
			virtual int Height() const = 0;

			virtual int& HeightRef()  = 0;

			/**
			 * @brief Checks if the keyboard key is pressed.
			 * 
			 * @param key : The keyboard key pressed.
			 * @return true if the key is pressed at the current frame.
			 * @return false otherwise.
			 */
			virtual bool KeyPressed(int key) const = 0;

			/**
			 * @brief Checks if the keyboard key is pressed.
			 * 
			 * @param key : The keyboard key released.
			 * @return true if the key is released at the current frame.
			 * @return false otherwise.
			 */
			virtual bool KeyReleased(int key) const = 0;

			/**
			 * @brief Checks if the mouse key is pressed.
			 * 
			 * @param key : The mouse key pressed.
			 * @return true if the key is pressed at the current frame.
			 * @return false otherwise.
			 */
			virtual bool MousePressed(int key) const = 0;

			/**
			 * @brief Checks if the mouse key is released.
			 * 
			 * @param key : The mouse key released.
			 * @return true if the key is released at the current frame.
			 * @return false otherwise.
			 */
			virtual bool MouseReleased(int key) const = 0;

			/**
			 * @brief Adds a callback 
			 * 
			 * @param type 
			 * @param callback 
			 */
			virtual void SetCallback(CallbackType type, void *callback) = 0;
	};
}