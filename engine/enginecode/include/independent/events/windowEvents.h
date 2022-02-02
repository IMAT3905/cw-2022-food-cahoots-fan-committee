/* \file windowsEvents.h */
#pragma once

#include "event.h"

#include <glm/glm.hpp>

namespace Engine
{
	/** \class WindowCloseEvent
	** \brief A class that is used when the window is closed
	*/
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}; //!< Constructor for when the window is closed
		static EventType getStaticType() { return EventType::WindowClose; } //!< Returns the static type
		virtual inline EventType getEventType() const override { return EventType::WindowClose; } //!< Returns the event type
		virtual int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flag
	};

	/** \class WindowResizeEvent
	** \brief A class that is used when the window is resized
	*/
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int32_t width, int32_t height) : m_width(width), m_height(height) {} //!< Constructor for when the window is resized \param width is the new width of the window \param height is the new height of the window
		static EventType getStaticType() { return EventType::WindowResize; } //!< Returns the static type
		virtual inline EventType getEventType() const override { return EventType::WindowResize; } //!< Get the event type
		virtual int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Get the event category flag

		inline int32_t getWidth() const { return m_width; } //!< Returns the width of the window
		inline int32_t getHeight() const { return m_height; } //!< Returns the height of the window
		inline glm::ivec2 getSize() const { return { m_width, m_height }; } //!< Returns the size of the window
	private:
		int32_t m_width; //!< Window width after resize
		int32_t m_height; //!< Window height after resize
	};

	/** \class WindowFocusEvent
	** \brief A class that is used when the window enters focus
	*/
	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {} //!< Constructor for when the window is put in focus
		static EventType getStaticType() { return EventType::WindowFocus; } //!< Returns the static type
		virtual inline EventType getEventType() const override { return EventType::WindowFocus; } //!< Returns the event type
		virtual inline int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flags
	};

	/** \class WindowLostFocusEvent
	** \brief A class that is used when the window loses focus
	*/
	class WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {} //!< Constructor for when the window loses focus
		static EventType getStaticType() { return EventType::WindowLostFocus; } //!< Returns the static type
		virtual inline EventType getEventType() const override { return EventType::WindowLostFocus; } //!< Returns the event type
		virtual inline int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flags
	};

	/** \class WindowMovedEvent
	** \brief A class that is used when the window is moved
	*/
	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int32_t x, int32_t y) : m_xPos(x), m_yPos(y) {} //!< Constructor for when the window is moved \param x is the position on the x axis \param y is the position on the y axis
		static EventType getStaticType() { return EventType::WindowMoved; } //!< Returns the static type
		virtual inline EventType getEventType() const override { return EventType::WindowMoved; } //!< Returns the event type
		virtual inline int32_t getCategoryFlags() const override { return EventCategoryWindow; } //!< Returns the category flags

		inline int32_t getXPos() const { return m_xPos; } //!< Returns the position of the window on the x axis
		inline int32_t getYPos() const { return m_yPos; } //!< Returns the position of the window on the y axis
		inline glm::ivec2 getPos() const { return { m_xPos, m_yPos }; } //!< Returns the position of the window
	private:
		int32_t m_xPos; //!< The position of the window on the x axis
		int32_t m_yPos; //!< The position of the window on the y axis
	};
}