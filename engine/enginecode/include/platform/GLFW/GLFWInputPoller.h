/* \file GLFWInputPoller.h */
#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Engine
{
	/** \class GLFWInputPoller
	** \brief Input poller for getting current keyboard/mouse state specific to the GLFW system
	*/
	class GLFWInputPoller
	{
	public:
		static bool isKeyPressed(int32_t keyCode); //!< Check if the key is pressed \param keyCode is the key that has been pressed
		static bool isMouseButtonPressed(int32_t mouseButton); //!< Check if the mouse button has been pressed \param mouseButton is the mouse button that has been pressed
		static glm::vec2 getMousePosition(); //!< current mouse position
		static void setCurrentWindow(GLFWwindow* newWin) { s_window = newWin; } //!< Set a window as current \param newWin is the new window to become the current window
	private:
		static GLFWwindow* s_window; //!< Current GLFW window
	};
}