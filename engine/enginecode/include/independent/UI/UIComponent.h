#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Engine {
	struct UIColourScheme {
		glm::vec4 beckground = { 0.2f,0.2f,0.2f,1.0f };
		glm::vec4 foreground = { 1.0f,1.0f,1.0f,1.0f };
		glm::vec4 highlight = { 0.4f,0.2f,0.6f,1.0f };
	};

	class UIComponent {
	public:
		glm::ivec2 GetPosition() { return m_position; }
		glm::ivec2 GetSize() { return m_size; }
		virtual void UpdateSize() = 0;
		virtual void UpdatePosition() = 0;
		virtual void OnRender() const {};
		virtual void OnMouseMove(glm::ivec2& mousepos) {};
		virtual void OnMousePress(glm::ivec2& mousepos, int32_t button) {};
		void SetParent(UIComponent* parent) { m_parent = parent; }
		UIComponent* GetParent() const { return m_parent; }
		UIColourScheme* m_colourscheem = nullptr;
	protected:
		glm::ivec2 m_position;
		glm::ivec2 m_size;
		UIComponent* m_parent = nullptr;
	};
}