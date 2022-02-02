/** \file eventHandlerTests.cpp */

#include "eventHandlerTests.h"

TEST(WindowEventHandler, onClose)
{
	MockApplication app;
	Engine::WindowCloseEvent e;

	bool handledBefore = e.handled();

	auto& onCloseFunctionDefault = app.m_handler.getOnCloseCallback();
	onCloseFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onCloseFunction = app.m_handler.getOnCloseCallback();
	onCloseFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(WindowEventHandler, ResizeConstructor)
{
	MockApplication app;
	Engine::WindowResizeEvent e(800, 600);

	bool handledBefore = e.handled();

	auto& onResizeFunctionDefault = app.m_handler.getOnResizeCallback();
	onResizeFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onResizeFunction = app.m_handler.getOnResizeCallback();
	onResizeFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(WindowEventHandler, WindowFocusConstructor)
{
	MockApplication app;
	Engine::WindowFocusEvent e;

	bool handledBefore = e.handled();

	auto& onFocusFunctionDefault = app.m_handler.getOnFocusCallback();
	onFocusFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onFocusFunction = app.m_handler.getOnFocusCallback();
	onFocusFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(WindowEventHandler, WindowLostFocusConstructor)
{
	MockApplication app;
	Engine::WindowLostFocusEvent e;

	bool handledBefore = e.handled();

	auto& onLostFocusFunctionDefault = app.m_handler.getOnLostFocusCallback();
	onLostFocusFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onLostFocusFunction = app.m_handler.getOnLostFocusCallback();
	onLostFocusFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(WindowEventHandler, WindowMovedConstructor)
{
	MockApplication app;
	Engine::WindowMovedEvent e(100, 50);

	bool handledBefore = e.handled();

	auto& onWindowMovedFunctionDefault = app.m_handler.getOnWindowMovedCallback();
	onWindowMovedFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onWindowMovedFunction = app.m_handler.getOnWindowMovedCallback();
	onWindowMovedFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(KeyboardEventHandler, KeyPressedConstructor)
{
	MockApplication app;
	Engine::KeyPressedEvent e(15, 5);

	bool handledBefore = e.handled();

	auto& onKeyPressedFunctionDefault = app.m_handler.getOnKeyPressedCallback();
	onKeyPressedFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onKeyPressedFunction = app.m_handler.getOnKeyPressedCallback();
	onKeyPressedFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(KeyboardEventHandler, KeyReleasedConstructor)
{
	MockApplication app;
	Engine::KeyReleasedEvent e(20);

	bool handledBefore = e.handled();

	auto& onKeyReleasedFunctionDefault = app.m_handler.getOnKeyReleasedCallback();
	onKeyReleasedFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onKeyReleasedFunction = app.m_handler.getOnKeyReleasedCallback();
	onKeyReleasedFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(KeyboardEventHandler, KeyTypedConstructor)
{
	MockApplication app;
	Engine::KeyTypedEvent e(20);

	bool handledBefore = e.handled();

	auto& onKeyTypedFunctionDefault = app.m_handler.getOnKeyTypedCallback();
	onKeyTypedFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onKeyTypedFunction = app.m_handler.getOnKeyTypedCallback();
	onKeyTypedFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(MouseEventHandler, MouseMovedConstructor)
{
	MockApplication app;
	Engine::MouseMovedEvent e(100.f, 150.f);

	bool handledBefore = e.handled();

	auto& onMouseMovedFunctionDefault = app.m_handler.getOnMouseMovedCallback();
	onMouseMovedFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onMouseMovedFunction = app.m_handler.getOnMouseMovedCallback();
	onMouseMovedFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(MouseEventHandler, MouseScrolledConstructor)
{
	MockApplication app;
	Engine::MouseScrolledEvent e(20, 50.f);

	bool handledBefore = e.handled();

	auto& onMouseScrolledFunctionDefault = app.m_handler.getOnMouseWheelCallback();
	onMouseScrolledFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onMouseScrolledFunction = app.m_handler.getOnMouseWheelCallback();
	onMouseScrolledFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(MouseEventHandler, MouseButtonPressedConstructor)
{
	MockApplication app;
	Engine::MouseButtonPressedEvent e(1);

	bool handledBefore = e.handled();

	auto& onMouseButtonPressedFunctionDefault = app.m_handler.getOnButtonPressedCallback();
	onMouseButtonPressedFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onMouseButtonPressedFunction = app.m_handler.getOnButtonPressedCallback();
	onMouseButtonPressedFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}

TEST(MouseEventHandler, MouseButtonReleasedConstructor)
{
	MockApplication app;
	Engine::MouseButtonReleasedEvent e(2);

	bool handledBefore = e.handled();

	auto& onMouseButtonReleasedFunctionDefault = app.m_handler.getOnButtonReleasedCallback();
	onMouseButtonReleasedFunctionDefault(e);

	bool handledDefault = e.handled();

	app.setCallback();
	auto& onMouseButtonReleasedFunction = app.m_handler.getOnButtonReleasedCallback();
	onMouseButtonReleasedFunction(e);

	bool handledAfter = e.handled();

	EXPECT_EQ(false, handledDefault);
	EXPECT_EQ(handledBefore, handledDefault);
	EXPECT_NE(handledBefore, handledAfter);
	EXPECT_EQ(handledAfter, true);
	EXPECT_EQ(handledBefore, false);
}