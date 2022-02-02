/** \file event.h */

#pragma once

#include <intTypes.h>

namespace Engine
{
	/** \enum EventType
	** \brief An enum for event types 
	*/
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	/** \enum EventCategory
	** \brief Enum for type category flags
	*/
	enum EventCategory
	{
		None = 0,
		EventCategoryWindow = 1 << 0,		//00000001
		EventCategoryInput = 1 << 1,		//00000010
		EventCategoryKeyboard = 1 << 2,		//00000100
		EventCategoryMouse = 1 << 3,		//00001000
		EventCategoryMouseButton = 1 << 4	//00010000
	};

	/** \class Event
	** \brief Base class for events
	*/
	class Event
	{
	public:
		virtual EventType getEventType() const = 0; //!< Get the event type
		virtual int32_t getCategoryFlags() const = 0; //!< Get the event category flags
		inline bool handled() const { return m_handled; } //!< Checks if the event has been handled
		inline void handle(bool isHandled) { m_handled = isHandled; } //!< Handle the event \param isHandled is whether the event has been handled
		inline bool isInCategory(EventCategory category) const { return getCategoryFlags() & category;  } //!< Checks if the event is in category \param category will checked if it is in the category list
	protected:
		bool m_handled = false; //!< Has the event been handled
	};
}
