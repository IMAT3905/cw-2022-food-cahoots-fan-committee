#include "engine_pch.h"
#include "UI/ModalWindow.h"
#include "UI/Containers.h"

namespace Engine {
	int32_t ModalWindow::padding = 6;

	ModalWindow::ModalWindow() {
		glm::ivec2 winsize = glm::ivec2(Application::getInstance().getWindow()->getWidth(), Application::getInstance().getWindow()->getHeight());
		m_position = (winsize / 2);
		m_size = (glm::ivec2(0.0f));
		m_windowcolourscheme = (UIColourScheme());
		m_colourscheme = &m_windowcolourscheme;
		m_container = RootContainer(this);
		m_container.UpdatePosition(m_position + glm::ivec2(ModalWindow::padding, ModalWindow::padding));
	}

	void ModalWindow::OnRender() const {
		if (m_active) {
			glm::ivec2 winsize = glm::ivec2(Application::getInstance().getWindow()->getWidth(), Application::getInstance().getWindow()->getHeight());

			glm::vec4 bgcover = m_windowcolourscheme.background;
			bgcover.a = 0.5f;
			//create top left size
			//create top left size
		}
	}

	void ModalWindow::AddContainer(HorizontalContainer& container) {
		m_container.AddContainer(container);
	}

	void ModalWindow::UpdateSize() {
		glm::ivec2 childsize = m_container.GetSize();

		m_size.x = std::max(childsize.x + (padding * 2), m_size.x);
		m_size.y = std::max(childsize.y + (padding * 2), m_size.y);

		glm::ivec2 winsize = glm::ivec2(Application::getInstance().getWindow()->getWidth(), Application::getInstance().getWindow()->getHeight());
		
		m_position = (winsize - m_size) / 2;
		m_container.UpdatePosition(m_position + glm::ivec2(ModalWindow::padding, ModalWindow::padding));
	}

	void ModalWindow::OnMousePress(glm::ivec2& mousepos, int32_t button) {

	}
}