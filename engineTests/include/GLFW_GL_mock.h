/** \file GLFW_GL_mock.h */

#pragma once

#include "include/platform/GLFW/GLFWSystem.h"
#include "include/platform/GLFW/GLFWWindowImpl.h"

/** \class GLFW_GL_mock
** \brief Class to run a mock application
*/
class GLFW_GL_mock
{
public:
	GLFW_GL_mock(); //!< Constructor 
	~GLFW_GL_mock(); //!< Destructor
	std::shared_ptr<Engine::System> glfwSystem; //!< Pointer to system
	std::shared_ptr<Engine::Window> glfwWindow; //!< Pointer to window
};