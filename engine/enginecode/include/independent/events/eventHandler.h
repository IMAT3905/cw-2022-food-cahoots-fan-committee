/** \file eventHandler.h */
#pragma once

#include "events.h"

#include <functional>

namespace Engine
{
	/** \class EventHandler
	** \brief A class for handling events
	*/
	class EventHandler
	{
	public:
		void setOnCloseCallback(const std::function<bool(WindowCloseEvent&)>& fn) { m_onCloseCallback = fn; } //!< This is set for when the window is closed \param fn is the function for this event
		void setOnResizeCallback(const std::function<bool(WindowResizeEvent&)>& fn) { m_onResizeCallback = fn; } //!< This is set for when the window is resized \param fn is the function for this event
		void setOnFocusCallback(const std::function<bool(WindowFocusEvent&)>& fn) { m_onFocusCallback = fn; } //!< This is set for when the window is focused \param fn is the function for this event
		void setOnLostFocusCallback(const std::function<bool(WindowLostFocusEvent&)>& fn) { m_onLostFocusCallback = fn; } //!< This is set for when the window loses focus \param fn is the function for this event
		void setOnWindowMovedCallback(const std::function<bool(WindowMovedEvent&)>& fn) { m_onWindowMovedCallback = fn; } //!< This is set for when the window is moved \param fn is the function for this event
		void setOnKeyPressedCallback(const std::function<bool(KeyPressedEvent&)>& fn) { m_onKeyDownCallback = fn; } //!< This is set for when a key is pressed \param fn is the function for this event
		void setOnKeyReleasedCallback(const std::function<bool(KeyReleasedEvent&)>& fn) { m_onKeyUpCallback = fn; } //!< This is set for when a key is released \param fn is the function for this event
		void setOnKeyTypedCallback(const std::function<bool(KeyTypedEvent&)>& fn) { m_onKeyTypedCallback = fn; } //!< This is set for when a key is typed \param fn is the function for this event
		void setOnButtonPressedCallback(const std::function<bool(MouseButtonPressedEvent&)>& fn) { m_onMouseDownCallback = fn; } //!< This is set for when a mouse button is pressed \param fn is the function for this event
		void setOnButtonReleasedCallback(const std::function<bool(MouseButtonReleasedEvent&)>& fn) { m_onMouseUpCallback = fn; } //!< This is set for when a mouse button is released is pressed \param fn is the function for this event
		void setOnMouseMovedCallback(const std::function<bool(MouseMovedEvent&)>& fn) { m_onMouseMovedCallback = fn; } //!< This is set for when the mouse is moved\param fn is the function for this event 
		void setOnMouseWheelCallback(const std::function<bool(MouseScrolledEvent&)>& fn) { m_onMouseWheelCallback = fn; } //!< This is set for when the mouse wheel is scrolled \param fn is the function for this event
		//void setOnMonitorConnectedCallback(const std::function<bool())

		std::function<bool(WindowCloseEvent&)>& getOnCloseCallback() { return m_onCloseCallback; } //!< Return onCloseCallback
		std::function<bool(WindowResizeEvent&)>& getOnResizeCallback() { return m_onResizeCallback; } //!< Return onResizeCallback
		std::function<bool(WindowFocusEvent&)>& getOnFocusCallback() { return m_onFocusCallback; } //!< Return onFocusCallback
		std::function<bool(WindowLostFocusEvent&)>& getOnLostFocusCallback() { return m_onLostFocusCallback; } //!< Return onLostFocusCallback
		std::function<bool(WindowMovedEvent&)>& getOnWindowMovedCallback() { return m_onWindowMovedCallback; } //!< Return onWindowMovedCallback
		std::function<bool(KeyPressedEvent&)>& getOnKeyPressedCallback() { return m_onKeyDownCallback; } //!< Return onKeyDownCallback
		std::function<bool(KeyReleasedEvent&)>& getOnKeyReleasedCallback() { return m_onKeyUpCallback; } //!< Return onKeyUpCallback
		std::function<bool(KeyTypedEvent&)>& getOnKeyTypedCallback() { return m_onKeyTypedCallback; } //!< Return onKeyTypedCallback
		std::function<bool(MouseButtonPressedEvent&)>& getOnButtonPressedCallback() { return m_onMouseDownCallback; } //!< Return onMouseDownCallback
		std::function<bool(MouseButtonReleasedEvent&)>& getOnButtonReleasedCallback() { return m_onMouseUpCallback; } //!< Return onMouseUpCallback
		std::function<bool(MouseMovedEvent&)>& getOnMouseMovedCallback() { return m_onMouseMovedCallback; } //!< Return onMouseMovedCallback 
		std::function<bool(MouseScrolledEvent&)>& getOnMouseWheelCallback() { return m_onMouseWheelCallback; } //!< Return onMouseWheelCallback
	private:
		std::function<bool(WindowCloseEvent&)> m_onCloseCallback = std::bind(&EventHandler::defaultOnClose, this, std::placeholders::_1); //!< The default for a window close event is set
		std::function<bool(WindowResizeEvent&)> m_onResizeCallback = std::bind(&EventHandler::defaultOnResize, this, std::placeholders::_1); //!< The default for a window resize event is set
		std::function<bool(WindowFocusEvent&)> m_onFocusCallback = std::bind(&EventHandler::defaultOnFocus, this, std::placeholders::_1); //!< The default for a window focus event is set
		std::function<bool(WindowLostFocusEvent&)> m_onLostFocusCallback = std::bind(&EventHandler::defaultOnLostFocus, this, std::placeholders::_1); //!< The default for a window lost focus event is set
		std::function<bool(WindowMovedEvent&)> m_onWindowMovedCallback = std::bind(&EventHandler::defaultOnWindowMoved, this, std::placeholders::_1); //!< The default for a window moved event is set
		std::function<bool(KeyPressedEvent&)> m_onKeyDownCallback = std::bind(&EventHandler::defaultOnKeyPress, this, std::placeholders::_1); //!< The default for a key pressed event is set
		std::function<bool(KeyReleasedEvent&)> m_onKeyUpCallback = std::bind(&EventHandler::defaultOnKeyRelease, this, std::placeholders::_1); //!< The default for a key released event is set
		std::function<bool(KeyTypedEvent&)> m_onKeyTypedCallback = std::bind(&EventHandler::defaultOnKeyTyped, this, std::placeholders::_1); //!< The default for a key typed event is set
		std::function<bool(MouseButtonPressedEvent&)> m_onMouseDownCallback = std::bind(&EventHandler::defaultOnMouseDown, this, std::placeholders::_1); //!< The default for a mouse down event is set
		std::function<bool(MouseButtonReleasedEvent&)> m_onMouseUpCallback = std::bind(&EventHandler::defaultOnMouseUp, this, std::placeholders::_1); //!< The default for a mouse up event is set
		std::function<bool(MouseMovedEvent&)> m_onMouseMovedCallback = std::bind(&EventHandler::defaultOnMouseMoved, this, std::placeholders::_1); //!< The default for a mouse moved event is set
		std::function<bool(MouseScrolledEvent&)> m_onMouseWheelCallback = std::bind(&EventHandler::defaultOnMouseWheel, this, std::placeholders::_1); //!< The default for a mouse wheel event is set

		bool defaultOnClose(WindowCloseEvent& e) { return false; } //!< The default state for the window being closed \param e is the type of event
		bool defaultOnResize(WindowResizeEvent& e) { return false; } //!< The default state for the window being resized \param e is the type of event
		bool defaultOnFocus(WindowFocusEvent& e) { return false; } //!< The default state for the window being focused on \param e is the type of event
		bool defaultOnLostFocus(WindowLostFocusEvent& e) { return false; } //!< The default state for the window losing focus \param e is the type of event
		bool defaultOnWindowMoved(WindowMovedEvent& e) { return false; } //!< The default state for the window being moved \param e is the type of event
		bool defaultOnKeyPress(KeyPressedEvent& e) { return false; } //!< The default state for a key being pressed \param e is the type of event
		bool defaultOnKeyRelease(KeyReleasedEvent& e) { return false; } //!< The default state for a key being released \param e is the type of event
		bool defaultOnKeyTyped(KeyTypedEvent& e) { return false; } //!< The default state for a key being typed \param e is the type of event
		bool defaultOnMouseDown(MouseButtonPressedEvent& e) { return false; } //!< The default state for a mouse button being pressed \param e is the type of event
		bool defaultOnMouseUp(MouseButtonReleasedEvent& e) { return false; } //!< The default state for a mouse button being released \param e is the type of event
		bool defaultOnMouseMoved(MouseMovedEvent& e) { return false; } //!< The default state for the mouse being moved \param e is the type of event
		bool defaultOnMouseWheel(MouseScrolledEvent& e) { return false; } //!< The default state for the mouse wheel being scrolled \param e is the type of event
	};
}
