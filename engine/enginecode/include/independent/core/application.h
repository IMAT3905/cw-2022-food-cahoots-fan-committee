/* \file application.h */
#pragma once

#include "systems/log.h"
#include "timer.h"
#include "events/events.h"
#include "events/eventHandler.h"
#include "core/window.h"
#include "core/inputPoller.h"
#include "cameras/cameraController.h"
#include "core/layerStack.h"
#include <entt/entt.hpp>
#include "systems/audio.h"

namespace Engine {

	/** \class Application
	** \brief Fundamental class of the engine. A singleton which runs the game loop infinitely.
	*/

	struct ApplicationProps
	{
		WindowProperties winProps;
		uint32_t vertexCapacity3D = 4000;
		uint32_t indexCapacity3D = 1000;
		uint32_t batchSize3D = 4000;
		uint32_t batchSize2D = 8192;
		const char* fontFilePath = "./assets/fonts/arial.ttf";
		uint32_t characterSize = 86;
	};

	class Application
	{
	protected:
		Application(ApplicationProps props); //!< Constructor

		std::shared_ptr<Log> m_logSystem; //!< Log system		
		std::shared_ptr<System> m_windowsSystem; //!< Windows system
		std::shared_ptr<AudioSystem> m_audioSystem; //!< Audio System

		std::shared_ptr<Window> m_window; //!< Window
		std::shared_ptr<Timer> m_timer; //!< Timer

		LayerStack m_layerStack;

		bool onClose(WindowCloseEvent& e); //!< Run when the window closes \param e which is the type of event
		bool onResize(WindowResizeEvent& e); //!< Run when the window is resized \param e which is the type of event
		bool onMoved(WindowMovedEvent& e); //!< Run when the window is moved \param e which is the type of event
		bool onGainFocus(WindowFocusEvent& e); //!< Run when the window becomes the focus \param e which is the type of event
		bool onLostFocus(WindowLostFocusEvent& e); //!< Run when the focus on the window is lost \param e which is the type of event
		bool onKeyPressed(KeyPressedEvent& e); //!< Run when a key is pressed \param e which is the type of event
		bool onKeyReleased(KeyReleasedEvent& e); //!< Run when a key is released \param e which is the type of event
		bool onMousePressed(MouseButtonPressedEvent& e); //!< Run when a mouse button is pressed \param e which is the type of event
		bool onMouseReleased(MouseButtonReleasedEvent& e); //!< Run when a mouse button is released \param e which is the type of event
		bool onMouseWheel(MouseScrolledEvent& e); //!< Run when the mouse wheel is scrolled \param e which is the type of event
		bool onMouseMoved(MouseMovedEvent& e); //!< Run when the mouse is moved	\param e which is the type of event
		//bool onMonitorConnected(Monitor)
	private:
		static Application* s_instance; //!< Singleton instance of the application
		bool m_running = true; //!< Is the application running?	
		entt::registry m_registry; //!< ECS
		std::vector<entt::entity> m_entities; //!< The entities
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		void run(); //!< Main loop	
		inline std::shared_ptr<Window> getWindow() { return m_window; }
		inline std::vector<entt::entity>& getEntities() { return m_entities; }
		inline entt::registry& getRegistry() { return m_registry; }
		inline std::shared_ptr<AudioSystem> GetAudio() { return m_audioSystem; }
	};

	Application* startApplication(); //!< Function definition which provides an entry hook
}