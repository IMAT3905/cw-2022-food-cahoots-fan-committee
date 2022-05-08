/** \file GLFWWindowImpl.h */
#pragma once

#include "core/window.h"
#include <GLFW/glfw3.h>

namespace Engine
{
	/** \class GLFWWindowImpl
	** \brief Implementation of a window using GLFW
	*/

	class GLFWWindowImpl : public Window
	{
	public:
		GLFWWindowImpl(const WindowProperties& properties); //!< Constructor \param properties are the properties associated with the window
		virtual void init(const WindowProperties& properties) override; //!< initialise the window \param properties are the properties associated with the window
		virtual void close() override; //!< Close the window
		//virtual ~Window() {}; //!< Destructor for the window
		virtual void onUpdate(float timestep) override; //!< Update function \param timestep is the current amount of time passed
		virtual void onResize(WindowResizeEvent& e) override; //!< Resize function \param e is the window resize event
		virtual void setVSync(bool VSync) override; //!< setting vsync \param VSync is for whether vsync will be set or not
		virtual inline unsigned int getWidth() const override { return m_props.width; } //!< returning width
		virtual inline unsigned int getHeight() const override { return m_props.height; } //!< returning height
		virtual inline float getWidthf() const override { return static_cast<float>(m_props.width); } //!< returning width as a float
		virtual inline float getHeightf() const override { return static_cast<float>(m_props.height); } //!< return height as a float
		virtual inline void* getNativeWindow() const override { return m_native; } //!< return the platform
		virtual inline bool isFullScreenMode() const override { return m_props.isFullScreen; } //!< checking if the window is fullscreen
		virtual inline bool isVSync() const override { return m_props.isVSync; } //!< checking if vsync is enabled
		virtual void setFullScreenMode(bool fullscreenState) override; //!< Changes between fullscreen and windowed \param fullscreenState is if fullscreen is currently enabled or not
	private:
		WindowProperties m_props; //!< Properties
		GLFWwindow* m_native; //<! Native GLFW window
		GLFWmonitor* m_monitor; //!< native monitor on which the window is rendered
		float m_aspectRatio; //<! Aspect ratio
		glm::ivec2 m_nonFullscreenPos; //!< Last position of the window when not in fullscreen mode
		glm::ivec2 m_nonFullscreenSize; //!< Last size of the window when not in fullscreen mode
	};
}