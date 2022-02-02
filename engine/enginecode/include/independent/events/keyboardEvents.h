/** \file keyboardEvents.h */
#pragma once

#include "event.h"

namespace Engine
{
	/** \class KeyEvent
	** \brief The main key class that each key subclass will inherit from
	*/
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(int32_t keycode) : m_keyCode(keycode) {} //!< Constructor for what key is pressed \param keycode the key that is pressed
		int32_t m_keyCode; //!< the key that is pressed
	public:
		inline int32_t getKeyCode() const { return m_keyCode; } //!< Returns the key that is pressed
		virtual inline int32_t getCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; } //!< Returns the category flags
	};

	/** \class KeyPressedEvent
	** \brief A class that is used when a key is pressed
	*/
	class KeyPressedEvent : public KeyEvent
	{
	private:
		int32_t m_repeatCount; //!< the amount of times a key is repeatedly pressed
	public:
		KeyPressedEvent(int32_t keycode, int32_t repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {} //!< Constructor for a key pressed event \param keycode is the key that is pressed \param repeatCount is the number of times a key is repeated

		inline int32_t getRepeatCount() const { return m_repeatCount; } //!< Returns how many times a key is repeatedly pushed
		static EventType getStaticType() { return EventType::KeyPressed; } //!< Returns the event type
		virtual inline EventType getEventType() const override { return getStaticType();  } //!< Returns the static type
	};

	/** \class KeyReleasedEvent
	** \brief A class that is used when a key is released
	*/
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int32_t keycode) : KeyEvent(keycode) {} //!< Constructor for a key released event \param keycode is the key that is released

		static EventType getStaticType() { return EventType::KeyReleased; } //!< Returns the event type
		virtual inline EventType getEventType() const override { return getStaticType(); } //!< Returns the static type
	};

	/** \class KeyTypedEvent
	** \brief A class that is used when a key is typed
	*/
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int32_t keycode) : KeyEvent(keycode) {} //!< Constructor for a key typed event \param keycode is the key that is typed

		static EventType getStaticType() { return EventType::KeyTyped; } //!< Returns the event type
		virtual inline EventType getEventType() const override { return getStaticType(); } //!< Returns the static type
	};
};
