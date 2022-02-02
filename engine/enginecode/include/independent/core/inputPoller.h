/* \file inputPoller.h */
#pragma once

#include <glm/glm.hpp>

namespace Engine
{
	/** \class InputPoller
	**  \brief API agnostic input poller for getting current keyboard/mouse state
	*/
	class InputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode); //!< Check if a key is pressed \param keyCode is the key that is pressed
		static bool isMouseButtonPressed(int32_t mouseButton); //!< Check if a mouse button is pressed \param mouseButton is the button on the mouse that is pressed
		static glm::vec2 getMousePosition(); //!< The current mouse position
		static void setNativeWindow(void* nativeWin); //!< Sets the native window \param nativeWin is the window that will be set to native
		inline static float getMouseX() { return getMousePosition().x; } //!< Returns the mouse's current x position 
		inline static float getMouseY() { return getMousePosition().y; } //!< Returns the mouse's current x position 
	};
}