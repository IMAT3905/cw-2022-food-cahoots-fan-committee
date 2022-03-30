/** \file application.cpp */

#include "engine_pch.h"
#include <glad/glad.h>
#include "core/application.h"

#ifdef NG_PLATFORM_WINDOWS
//#include "platform/windows/winTimer.h"
#include "platform/GLFW/GLFWSystem.h"
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rendering/renderer3D.h"
#include "rendering/renderer2D.h"

namespace Engine {

	// Set static vars
	Application* Application::s_instance = nullptr;

	Application::Application(ApplicationProps appProps)
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		// Start systems

		// Start log
		m_logSystem.reset(new Log);
		m_logSystem->start();

		// Start the windows System
#ifdef NG_PLATFORM_WINDOWS
		m_windowsSystem.reset(new GLFWSystem);
#endif
		m_windowsSystem->start();

		// Start timer (not a system)
/*#ifdef NG_PLATFORM_WINDOWS
		m_timer.reset(new WinTimer);
#else*/
		m_timer.reset(new ChronoTimer);
//#endif
		m_timer->start();

		WindowProperties props("My Game Engine", 1024, 800);
		
		//start the audio system
		m_audioSystem.reset(new AudioSystem);
		m_audioSystem->start();

		m_window.reset(Window::create(appProps.winProps));

		m_window->getEventHandler().setOnCloseCallback(std::bind(&Application::onClose, this, std::placeholders::_1));
		m_window->getEventHandler().setOnResizeCallback(std::bind(&Application::onResize, this, std::placeholders::_1));
		m_window->getEventHandler().setOnWindowMovedCallback(std::bind(&Application::onMoved, this, std::placeholders::_1));
		m_window->getEventHandler().setOnFocusCallback(std::bind(&Application::onGainFocus, this, std::placeholders::_1));
		m_window->getEventHandler().setOnLostFocusCallback(std::bind(&Application::onLostFocus, this, std::placeholders::_1));

		m_window->getEventHandler().setOnKeyPressedCallback(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
		m_window->getEventHandler().setOnKeyReleasedCallback(std::bind(&Application::onKeyReleased, this, std::placeholders::_1));

		m_window->getEventHandler().setOnMouseMovedCallback(std::bind(&Application::onMouseMoved, this, std::placeholders::_1));
		m_window->getEventHandler().setOnButtonPressedCallback(std::bind(&Application::onMousePressed, this, std::placeholders::_1));
		m_window->getEventHandler().setOnButtonReleasedCallback(std::bind(&Application::onMouseReleased, this, std::placeholders::_1));
		m_window->getEventHandler().setOnMouseWheelCallback(std::bind(&Application::onMouseWheel, this, std::placeholders::_1));

		//m_window->getEventHandler().setOnMonitorConnectedCallback(std::bind(&Window::onMonitorConnected, m_window.get(), std::placeholders::_1));

		InputPoller::setNativeWindow(m_window->getNativeWindow());

		m_timer->reset();
	}

	//Set of event functions related to user interaction with user feedback

	bool Application::onClose(WindowCloseEvent & e)
	{
		e.handle(true);
		m_running = false;
		return e.handled();
	}

	bool Application::onResize(WindowResizeEvent& e)
	{
		e.handle(true);
		auto& size = e.getSize();
		m_window->onResize(e);
		//Log::info("Window Resize event: ({0}, {1})", size.x, size.y);
		return e.handled();
	}

	bool Application::onMoved(WindowMovedEvent& e)
	{
		e.handle(true);
		auto& pos = e.getPos();
		//Log::info("Window Moved event: ({0}, {1})", pos.x, pos.y);
		return e.handled();
	}

	bool Application::onGainFocus(WindowFocusEvent& e)
	{
		e.handle(true);
		//Log::info("Gained focus");
		return e.handled();
	}

	bool Application::onLostFocus(WindowLostFocusEvent& e)
	{
		e.handle(true);
		//Log::info("Lost focus");
		return e.handled();
	}

	bool Application::onKeyPressed(KeyPressedEvent& e)
	{
		switch (e.getKeyCode())
		{
		case NG_KEY_ESCAPE:
			m_running = false;
			e.handle(true);
			break;
		case NG_KEY_F10:
			m_window->setFullScreenMode(!m_window->isFullScreenMode());
			e.handle(true);
			break;
		default:
			m_layerStack.onKeyPressed(e);
		}
		e.handle(true);
		//Log::info("Key pressed event: {0}, repeat: {1}", e.getKeyCode(), e.getRepeatCount());
		//if (e.getKeyCode() == NG_KEY_SPACE) Log::error("SPACEBAR pressed");
		return e.handled();
	}

	bool Application::onKeyReleased(KeyReleasedEvent& e)
	{
		e.handle(true);
		//Log::info("Key released event: key: {0}", e.getKeyCode());
		//if (e.getKeyCode() == NG_KEY_UP) Log::error("UP released");
		return e.handled();
	}

	bool Application::onMousePressed(MouseButtonPressedEvent& e)
	{
		e.handle(true);
		//Log::info("Mouse pressed event: button: {0}", e.getButton());
		return e.handled();
	}

	bool Application::onMouseReleased(MouseButtonReleasedEvent& e)
	{
		e.handle(true);
		//Log::info("Mouse released event: button: {0}", e.getButton());
		return e.handled();
	}

	bool Application::onMouseWheel(MouseScrolledEvent& e)
	{
		e.handle(true);
		//Log::info("Mouse wheel event: {0}", e.getYOffset());
		return e.handled();
	}

	bool Application::onMouseMoved(MouseMovedEvent& e)
	{
		e.handle(true);
		auto& pos = e.getPos();
		//Log::info("Mouse moved event: ({0}, {1})", pos.x, pos.y);
		return e.handled();
	}

	Application::~Application()
	{
		//stop audio system
		m_audioSystem->stop();
		// Stop systems
		m_windowsSystem->stop();
		//Stop logger
		m_logSystem->stop();
		//Stop window system
	}

	void Application::run()
	{
		float timestep = 0.f;

	
		while (m_running)
		{
			timestep = m_timer->getElapsedTime();
			m_timer->reset();

			//Basic feedback for FPS, button presses and mouse position
			//Log::trace("FPS {0}", 1.0f / timestep);
			//if (InputPoller::isKeyPressed(NG_KEY_W)) Log::error("W Pressed");
			//if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_1)) Log::error("Left Mouse Button Pressed");
			//Log::trace("Current mouse pos: ({0}, {1})", InputPoller::getMouseX(), InputPoller::getMouseY());		

			m_layerStack.update(timestep);
			m_layerStack.render();

			m_window->onUpdate(timestep);
		};
	}
}