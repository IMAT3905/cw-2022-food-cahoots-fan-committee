#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "UI/UIComponent.h"
#include "UI/Widgets.h"

namespace Engine {
	class ModalWindow;
	class HorizontalContainer;
	
	class Container : public UIComponent {
	public:
		virtual bool IsEmpty() const = 0;
	};


	class RootContainer : public Container {
	public:
		RootContainer() = default;
		RootContainer(ModalWindow* parent);
		virtual void OnRender() const override;
		virtual bool IsEmpty() const override { return m_containers.empty(); }
		virtual void AddContainer(HorizontalContainer& container);
		void UpdatePosition(glm::ivec2& relativepos) override;
		void UpdateSize() override;

		std::vector<HorizontalContainer>::iterator begin() { return m_containers.begin(); }
		std::vector<HorizontalContainer>::iterator end() { return m_containers.end(); }
		std::vector<HorizontalContainer>::const_iterator begin() const { return m_containers.begin(); }
		std::vector<HorizontalContainer>::const_iterator end() const { return m_containers.end(); }
	private:
		std::vector<HorizontalContainer> m_containers;
	};
}