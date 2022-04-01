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

	class HorizontalContainer : public Container {
	public:
		bool IsEmpty() const override { return m_widgets.empty(); }
		template <typename G, typename ...Args>
		void AddWidget(Args && ... args) {
			std::shared_ptr<Widget> widget;
			widget.reset(new G(std::forward<Args>(args) ...));
			widget->SetParent(this);
			widget->m_colourscheme = m_colourscheme;
			m_widgets.push_back(widget);
			UpdateSize();
		}

		std::vector<std::shared_ptr<Widget>>::iterator getWidget(size_t index)
		{
			return m_widgets.begin() + index;
		}

		void UpdatePosition(glm::ivec2& relativepos) override;
		void UpdateSize() override;
		void OnRender() const override;

		std::vector<std::shared_ptr<Widget>>::iterator begin(){ return m_widgets.begin(); }
		std::vector<std::shared_ptr<Widget>>::iterator end() { return m_widgets.end(); }
		std::vector<std::shared_ptr<Widget>>::const_iterator begin() const { return m_widgets.begin(); }
		std::vector<std::shared_ptr<Widget>>::const_iterator end() const { return m_widgets.end(); }
	private:
		std::vector<std::shared_ptr<Widget>> m_widgets;
	};
}