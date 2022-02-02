/* \file window.h */
#pragma once

#include "events/eventHandler.h"
#include "core/graphicsContext.h"

namespace Engine
{
	/** \struct WindowProperties
	** \brief Abstract windows base class. All implemented windows should code to this interface.
	*/
	struct WindowProperties
	{
		char* title; //!< The window's title
		uint32_t width; //!< The window's width
		uint32_t height; //!< The window's height
		bool isFullScreen; //!< If the window is fullscreen
		bool isVSync; //!< If vsync is enabled

		WindowProperties(char* title = "My Window", uint32_t width = 800, uint32_t height = 600, bool fullscreen = false) : title(title), width(width), height(height), isFullScreen(fullscreen) {} //!< Constructor for setting the window's properties \param title sets the window's title \param width sets the window's width \param height sets the window's height \param fullscreen sets if the window is fullscreen
	};

	/** \class Window
	** \brief Abstract windows base class. All implemented windows should code to this interface.
	*/
	class Window
	{
	public:
		virtual void init(const WindowProperties& properties) = 0; //!< Initialise the window \param properties is the settings for the window
		virtual void close() = 0; //!< Close the window
		virtual ~Window() {}; //!< Destructor for the window
		virtual void onUpdate(float timestep) = 0; //!< Update function \param timestep is the current time passed
		virtual void onResize(WindowResizeEvent& e) = 0; //!< Resize function \param e is the window resize event
		virtual void setVSync(bool VSync) = 0; //!< Setting vsync \param VSync is if vsync is set
		virtual unsigned int getWidth() const = 0; //!< Returning width
		virtual unsigned int getHeight() const = 0; //!< Returning height
		virtual void* getNativeWindow() const = 0; //!< Return the platform
		virtual bool isFullScreenMode() const = 0; //!< Checking if the window is fullscreen
		virtual bool isVSync() const = 0; //!< Checking if vsync is enabled
		virtual void setFullScreenMode(bool fullscreenState) = 0; //!< Changes between fullscreen and windowed \param fullscreenState is if fullscreen is currently enabled or not

		inline virtual EventHandler& getEventHandler() { return m_handler; } //!< Return the event handler

		static Window* create(const WindowProperties& properties = WindowProperties()); //!< For creating the window \param properties is the settings for the window that will be initialised
	protected:
		EventHandler m_handler; //!< Event handler
		std::shared_ptr<GraphicsContext> m_graphicsContext; //!< Graphics context
	};
}