/* \file mouseEvents.h */
#pragma once

#include "event.h"
#include <glm/glm.hpp>

namespace Engine
{
	/** \class MouseEvent
	** \brief The main mouse class that each mouse subclass will inherit from
	*/
	class MouseEvent : public Event
	{
	public:
		virtual inline int32_t getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; } //!< Returns the category flags
	};

	/** \class MouseMovedEvent
	** \brief A class that is used when the mouse is moved
	*/
	class MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {} //!< Constructor for when the mouse is moved \param x the x position of the mouse \param y the y position of the mouse

		static EventType getStaticType() { return EventType::MouseMoved; } //!< Returns the event type
		virtual inline EventType getEventType() const override { return getStaticType(); } //!< Returns the static type
		inline float getX() const { return m_mouseX; }  //!< Returns the x position of the mouse
		inline float getY() const { return m_mouseY; } //!< Returns the y position of the mouse
		inline glm::vec2 getPos() const { return glm::vec2(m_mouseX, m_mouseY); } //!< Returns the x and y position of the mouse
	private:
		float m_mouseX, m_mouseY; //!< The mouse's x and y positions
	};

	/** \class MouseScrolledEvent
	** \brief A class that is used when the mouse wheel is scrolled
	*/
	class MouseScrolledEvent : public MouseEvent
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {} //!< Constructor for when the mouse wheel is scrolled \param xOffset is the offset in the x axis \param yOffset is the offset in the y axis

		static EventType getStaticType() { return EventType::MouseScrolled; } //!< Returns the event type
		virtual EventType getEventType() const override { return getStaticType(); } //!< Returns the static type
		inline float getXOffset() const { return m_xOffset; } //!< Returns the x offset
		inline float getYOffset() const { return m_yOffset; } //!< Returns the y offset
	private:
		float m_xOffset, m_yOffset; //!< The x and y offset
	};

	/** \class MouseButtonPressedEvent
	** \brief A class that is used when a mouse button is pressed
	*/
	class MouseButtonPressedEvent : public MouseEvent
	{
	public:
		MouseButtonPressedEvent(int32_t button) : m_button(button) {} //!< Constructor for when a mouse button is pressed \param button is the mouse button that is pressed

		static EventType getStaticType() { return EventType::MouseButtonPressed; } //!< Returns the event type
		virtual EventType getEventType() const override { return getStaticType(); } //!< Returns the static type
		inline int32_t getButton() const { return m_button; } //!< Returns the button pressed
	private:
		int32_t m_button; //!< The button that is pressed
	};

	/** \class MouseButtonReleasedEvent
	** \brief A class that is used when a mouse button is released
	*/
	class MouseButtonReleasedEvent : public MouseEvent
	{
	public:
		MouseButtonReleasedEvent(int32_t button) : m_button(button) {} //!< Constructor for when a mouse button is released \param button is the mouse button that is released

		static EventType getStaticType() { return EventType::MouseButtonReleased; } //!< Returns the event type 
		virtual EventType getEventType() const override { return getStaticType(); } //!< Returns the static type
		inline int32_t getButton() const { return m_button; } //!< Returns the button that is released
	private:
		int32_t m_button; //!< The button that is released
	};
};
