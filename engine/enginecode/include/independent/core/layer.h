/* \file layer.h */
#pragma once

#include <string>
#include "window.h"

namespace Engine
{
	class Layer
	{
	public:
		Layer(const char* name) : m_name(name) {}; //!< Constructor

		virtual void onAttach() {} //!< Runs when layer is attached
		virtual void onDetach() {} //!< Runs when layer is detached
		virtual void onRender() {} //!< Runs when layer is rendered
		virtual void onUpdate(float timestep) {} //!< Runs every frame
		virtual void onResize(WindowResizeEvent& e) {} //!< Runs when the layer's window is resized

		inline const char* getName() const { return m_name; } //!< Accessors
		inline void setDisplayed(bool displayStatus) { m_isDisplayed = displayStatus; }
		inline void setActive(bool activeStatus) { m_isActive = activeStatus; }
		inline void setFocused(bool focusedStatus) { m_isFocused = focusedStatus; }
		inline const bool isDisplayed() const { return m_isDisplayed; }
		inline const bool isActive() const { return m_isActive; }
		inline const bool isFocused() const { return m_isFocused; }

		virtual void onKeyPressed(KeyPressedEvent& e) { e.handle(false); } //!< On key press event
		virtual void onMouseMoved(MouseMovedEvent& e) { e.handle(false); }
		virtual void onMousePressed(MouseButtonPressedEvent& e) { e.handle(false); }
		virtual void onMouseReleased(MouseButtonReleasedEvent& e) { e.handle(false); }

		std::shared_ptr<Layer> scene = nullptr;
		std::shared_ptr<Layer> game = nullptr;
		std::shared_ptr<Layer> menu = nullptr;
		int scores[4] = { 0,0,0,0 };
		float selecttime = 5;
		int numselected[4] = { 0,0,0,0 };

	protected:
		const char* m_name; //!< Name, useful for debugging
		bool m_isDisplayed = true; //!< Should this layer be rendered
		bool m_isActive = true; //!< Should this layer be updated
		bool m_isFocused = true; //!< Should this layer be getting events
	};
}