#include <engine_pch.h>
#include "include/independent/UI/Widgets.h"
#include "include/independent/UI/Containers.h"
#include "include/independent/UI/ModalWindow.h"

namespace Engine {
	void Widget::UpdatePosition(glm::ivec2& relativepos) {
		m_position = relativepos;
	}

	void Widget::UpdateSize() {
		m_parent->UpdateSize();
	}

	Spacer::Spacer(glm::ivec2 size) {
		m_size = size;
		m_position = { 0,0 };
	}

	Spacer::Spacer(int32_t sizex, int32_t sizey) {
		m_size = glm::ivec2(sizex, sizey);
		m_position = { 0,0 };
	}

	Label::Label(glm::ivec2 size, const char* labeltext,Justification just) : m_text(labeltext), m_just(just) {
		m_size = size;
		m_position = { 0,0 };
	}

	Label::Label(int32_t sizex, int32_t sizey, const char* labeltext, Justification just) : m_text(labeltext), m_just(just) {
		m_size = glm::ivec2(sizex, sizey);
		m_position = { 0,0 };
		m_textpos = { 0,0 };
	}

	void Label::OnRender() const {
		Renderer2D::submit(m_text, m_textpos, m_colourscheme->foreground);
	}

	void Label::UpdatePosition(glm::ivec2& relativepos) {
		m_position = relativepos;
		glm::ivec2 textsize = Renderer2D::GetTextSize(m_text);
		glm::ivec2 centre = m_position + m_size / 2;
		switch (m_just)
		{
		case Engine::Justification::left:
			m_textpos.x = m_position.x;
			m_textpos.y = centre.y - textsize.y / 2;
			break;
		case Engine::Justification::centre:
			m_textpos.x = centre.x - textsize.x / 2;
			m_textpos.y = centre.y - textsize.y / 2;
			break;
		case Engine::Justification::right:
			m_textpos.x = (m_position.x+m_size.x) - textsize.x;
			m_textpos.y = centre.y - textsize.y / 2;
			break;
		default:
			break;
		}
	}

	void Button::OnRender() const {
		Renderer2D::submit(Quad::createTopLeftSize(m_position, m_size), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		Renderer2D::submit(m_text, m_textpos, glm::vec4(1.0f));
	}

	void Button::OnMousePress(glm::ivec2& mousepos, int32_t button) {
		if (button == NG_MOUSE_BUTTON_1) {
			bool clickedon =
				m_position.x <= mousepos.x && mousepos.x <= m_position.x + m_size.x &&
				m_position.y <= mousepos.y && mousepos.y <= m_position.y + m_size.y;
			if (clickedon) {
				m_callback();
			}
		}
	}

	ColouredSquare::ColouredSquare(int32_t sizex, int32_t sizey, glm::vec4 tint) : m_tint(tint){
		m_size = glm::ivec2(sizex, sizey);
		m_position = { 0,0 };
	}

	void ColouredSquare::OnRender() const {
		Renderer2D::submit(Quad::createTopLeftSize(m_position, m_size), m_tint);
	}
}