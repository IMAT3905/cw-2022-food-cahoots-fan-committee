#include "engine_pch.h"
#include "include/independent/UI/Containers.h"
#include "include/independent/UI/ModalWindow.h"

namespace Engine {
	RootContainer::RootContainer(ModalWindow* parent) {
		m_parent = parent;
		m_colourscheme = &parent->m_windowcolourscheme;
		m_position = { 0,0 };
		m_size = { ModalWindow::padding * 2,ModalWindow::padding * 2 };
	}

	void RootContainer::OnRender() const {
		for (auto& container : m_containers) container.OnRender();
	}

	void RootContainer::AddContainer(HorizontalContainer& container) {
		container.SetParent(this);
		container.m_colourscheme = m_colourscheme;
		for (auto& widget : container) { widget->m_colourscheme = m_colourscheme; }
		m_containers.push_back(container);
		UpdateSize();
	}

	void RootContainer::UpdatePosition(glm::ivec2& relativepos) {
		m_position = relativepos;
		glm::ivec2 childrelativepos = m_position + glm::ivec2(ModalWindow::padding, ModalWindow::padding);
		for (auto& container : m_containers) {
			container.UpdatePosition(childrelativepos);
			childrelativepos.y += container.GetSize().y;
		}
	}

	void RootContainer::UpdateSize() {
		glm::ivec2 size(0);
		size += 2 * glm::ivec2(ModalWindow::padding, ModalWindow::padding);
		for (auto& container : m_containers) {
			size.x = std::max(size.x, ModalWindow::padding * 2 + container.GetSize().x);
			size.y += container.GetSize().y;
		}

		m_size = size;

		m_parent->UpdateSize();
	}

	void HorizontalContainer::UpdatePosition(glm::ivec2& relativepos) {
		m_position = relativepos;

		glm::ivec2 childrelativepos = m_position + glm::ivec2(ModalWindow::padding, ModalWindow::padding);
		for (auto& widget : m_widgets) {
			widget->UpdatePosition(childrelativepos);
			childrelativepos.x += widget->GetSize().x;
		}
	}

	void HorizontalContainer::UpdateSize() {
		glm::ivec2 size(0);
		size += 2 * glm::ivec2(ModalWindow::padding, ModalWindow::padding);
		for (auto& widget : m_widgets) {
			size.x += widget->GetSize().x;
			size.y = std::max(size.y, ModalWindow::padding * 2 + widget->GetSize().y);
		}

		m_size = size;
		if (m_parent) m_parent->UpdateSize();
	}

	void HorizontalContainer::OnRender() const {
		for (auto widget : m_widgets) widget->OnRender();
	}
}