/** \file eventHandlerTests.h */

#pragma once

#include <gtest/gtest.h>
#include "events/eventHandler.h"

/** \class Mock Application
** \brief A class to run mock tests
*/
class MockApplication
{
public:
	void setCallback()
	{
		m_handler.setOnCloseCallback(std::bind(&MockApplication::onClose, this, std::placeholders::_1));
		m_handler.setOnResizeCallback(std::bind(&MockApplication::onResize, this, std::placeholders::_1));
		m_handler.setOnFocusCallback(std::bind(&MockApplication::onFocus, this, std::placeholders::_1));
		m_handler.setOnLostFocusCallback(std::bind(&MockApplication::onLostFocus, this, std::placeholders::_1));
		m_handler.setOnWindowMovedCallback(std::bind(&MockApplication::onWindowMoved, this, std::placeholders::_1));
		m_handler.setOnKeyPressedCallback(std::bind(&MockApplication::onKeyPressed, this, std::placeholders::_1));
		m_handler.setOnKeyReleasedCallback(std::bind(&MockApplication::onKeyReleased, this, std::placeholders::_1));
		m_handler.setOnKeyTypedCallback(std::bind(&MockApplication::onKeyTyped, this, std::placeholders::_1));
		m_handler.setOnMouseMovedCallback(std::bind(&MockApplication::onMouseMoved, this, std::placeholders::_1));
		m_handler.setOnMouseWheelCallback(std::bind(&MockApplication::onMouseScrolled, this, std::placeholders::_1));
		m_handler.setOnButtonPressedCallback(std::bind(&MockApplication::onMouseButtonPressed, this, std::placeholders::_1));
		m_handler.setOnButtonReleasedCallback(std::bind(&MockApplication::onMouseButtonReleased, this, std::placeholders::_1));
	}

	Engine::EventHandler m_handler;
private:
	bool onClose(Engine::WindowCloseEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onResize(Engine::WindowResizeEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onFocus(Engine::WindowFocusEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onLostFocus(Engine::WindowLostFocusEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onWindowMoved(Engine::WindowMovedEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onKeyPressed(Engine::KeyPressedEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onKeyReleased(Engine::KeyReleasedEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onKeyTyped(Engine::KeyTypedEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onMouseMoved(Engine::MouseMovedEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onMouseScrolled(Engine::MouseScrolledEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onMouseButtonPressed(Engine::MouseButtonPressedEvent& e)
	{
		e.handle(true);
		return e.handled();
	}

	bool onMouseButtonReleased(Engine::MouseButtonReleasedEvent& e)
	{
		e.handle(true);
		return e.handled();
	}
};