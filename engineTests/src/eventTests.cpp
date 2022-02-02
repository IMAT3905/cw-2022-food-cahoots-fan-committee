/** \file eventTests.cpp */

#include "eventTests.h"

TEST(WindowEvents, CloseConstructor)
{
	Engine::WindowCloseEvent e;
	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryWindow);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::WindowClose);
}

TEST(WindowEvents, ResizeConstructor)
{
	Engine::WindowResizeEvent e(800, 600);

	int32_t width1 = e.getWidth();
	int32_t height1 = e.getHeight();

	auto size = e.getSize();
	int32_t width2 = size.x;
	int32_t height2 = size.y;

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(width1, 800);
	EXPECT_EQ(width2, 800);
	EXPECT_EQ(height1, 600);
	EXPECT_EQ(height2, 600);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryWindow);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::WindowResize);
}

TEST(WindowEvents, WindowFocusConstructor)
{
	Engine::WindowFocusEvent e;
	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryWindow);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::WindowFocus);
}

TEST(WindowEvents, WindowLostFocusConstructor)
{
	Engine::WindowLostFocusEvent e;
	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryWindow);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::WindowLostFocus);
}

TEST(WindowEvents, WindowMovedConstructor)
{
	Engine::WindowMovedEvent e(100, 50);

	int32_t xPos1 = e.getXPos();
	int32_t yPos1 = e.getYPos();

	auto position = e.getPos();
	int32_t xPos2 = position.x;
	int32_t yPos2 = position.y;

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(xPos1, 100);
	EXPECT_EQ(xPos2, 100);
	EXPECT_EQ(yPos1, 50);
	EXPECT_EQ(yPos2, 50);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryWindow);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryWindow));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::WindowMoved);
}

TEST(KeyboardEvents, KeyPressedConstructor)
{
	Engine::KeyPressedEvent e(15, 5);

	int32_t repeatCount = e.getRepeatCount();
	int32_t keyCode = e.getKeyCode();

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(repeatCount, 5);
	EXPECT_EQ(keyCode, 15);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryKeyboard | Engine::EventCategory::EventCategoryInput);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryKeyboard));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::KeyPressed);
}

TEST(KeyboardEvents, KeyReleasedConstructor)
{
	Engine::KeyReleasedEvent e(20);

	int32_t keyCode = e.getKeyCode();

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(keyCode, 20);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryKeyboard | Engine::EventCategory::EventCategoryInput);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryKeyboard));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::KeyReleased);
}

TEST(KeyboardEvents, KeyTypedConstructor)
{
	Engine::KeyTypedEvent e(20);

	int32_t keyCode = e.getKeyCode();

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(keyCode, 20);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryKeyboard | Engine::EventCategory::EventCategoryInput);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryKeyboard));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::KeyTyped);
}

TEST(MouseEvents, MouseMovedConstructor)
{
	Engine::MouseMovedEvent e(100.f, 150.f);

	int32_t xPos1 = e.getX();
	int32_t yPos1 = e.getY();

	auto position = e.getPos();
	int32_t xPos2 = position.x;
	int32_t yPos2 = position.y;

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(xPos1, 100.f);
	EXPECT_EQ(xPos2, 100.f);
	EXPECT_EQ(yPos1, 150.f);
	EXPECT_EQ(yPos2, 150.f);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryMouse | Engine::EventCategory::EventCategoryInput);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryMouse));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::MouseMoved);
}

TEST(MouseEvents, MouseScrolledConstructor)
{
	Engine::MouseScrolledEvent e(20.f, 50.f);

	int32_t xOffset = e.getXOffset();
	int32_t yOffset = e.getYOffset();

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(xOffset, 20.f);
	EXPECT_EQ(yOffset, 50.f);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryMouse | Engine::EventCategory::EventCategoryInput);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryMouse));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::MouseScrolled);
}

TEST(MouseEvents, MouseButtonPressedConstructor)
{
	Engine::MouseButtonPressedEvent e(1);

	int32_t button = e.getButton();

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(button, 1);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryMouse | Engine::EventCategory::EventCategoryInput);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryMouse));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::MouseButtonPressed);
}

TEST(MouseEvents, MouseButtonReleasedConstructor)
{
	Engine::MouseButtonReleasedEvent e(2);

	int32_t button = e.getButton();

	int32_t cat = e.getCategoryFlags();
	Engine::EventType type = e.getEventType();
	Engine::EventType staticType = e.getStaticType();

	EXPECT_EQ(button, 2);
	EXPECT_EQ(cat, Engine::EventCategory::EventCategoryMouse | Engine::EventCategory::EventCategoryInput);
	EXPECT_TRUE(e.isInCategory(Engine::EventCategory::EventCategoryMouse));
	EXPECT_EQ(type, staticType);
	EXPECT_EQ(type, Engine::EventType::MouseButtonReleased);
}