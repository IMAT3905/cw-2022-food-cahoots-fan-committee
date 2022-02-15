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
}