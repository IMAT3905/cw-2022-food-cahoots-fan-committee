#pragma once

#include <glm/glm.hpp>
#include "rendering/renderer2D.h"
#include "UI/UIComponent.h"

namespace Engine {
	enum class Justification {left, centre, right};
	class HorizontalContainer;

	class Widget : public UIComponent {
	public:
		virtual void UpdatePosition(glm::ivec2& relativepos) override;
		virtual void UpdateSize() override;
	};

	class Spacer : public Widget {
	public:
		Spacer(glm::ivec2 size);
		Spacer(int32_t sizex, int32_t sizey);
		virtual void OnRender() const override {};
	};

	class Label : public Widget {
	public:
		Label(glm::ivec2 size, const char* labeltext, Justification just = Justification::centre);
		Label(int32_t sizex, int32_t sizey, const char* labeltext, Justification just = Justification::centre);
		virtual void OnRender() const;
		virtual void UpdatePosition(glm::ivec2& relativepos) override;
	protected:
		const char* m_text;
		glm::ivec2 m_textpos;
		Justification m_just;
	};

	class Button : public Label {
	public:
		Button(int32_t x, int32_t y, const char* buttontext, std::function<void(void)> OnClick) :
			Label(x, y, buttontext,Justification::left),
			m_callback(OnClick)
		{}
		Button(glm::ivec2 size, const char* buttontext, std::function<void(void)> OnClick) :
			Label(size,buttontext,Justification::left),
			m_callback(OnClick)
		{}
		virtual void OnRender() const override;
		virtual void OnMousePress(glm::ivec2& mousepos, int32_t button) override;
	private:
		std::function<void(void)> m_callback;
	};

	class ColouredSquare : public Widget {
	public:
		ColouredSquare(int32_t sizex, int32_t ysizey, glm::vec4 tint);
		virtual void OnRender() const override;
	private:
		glm::vec4 m_tint;
	};
}