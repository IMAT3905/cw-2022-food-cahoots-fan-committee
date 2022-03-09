#pragma once
#include "rendering/renderer2D.h"
#include "UI/UIComponent.h"
#include "core/application.h"
#include "UI/Containers.h"

namespace Engine {
	class ModalWindow : public UIComponent {
	public:
		ModalWindow();

		UIColourScheme m_windowcolourscheme;
		void show() { m_active = true; }
		void hide() { m_active = false; }
		void OnRender() const override;
		void AddContainer(HorizontalContainer& container);
		virtual void UpdateSize() override;
		virtual void UpdatePosition(glm::ivec2& relativepos) override {};

		virtual void OnMousePress(glm::ivec2& mousepos, int32_t button) override;
		virtual void OnMouseRelease(glm::ivec2& mousepos, int32_t button) override;
		virtual void OnMouseMove(glm::ivec2& mousepos) override;

		void ClearWindow();

		bool IsShown() { return m_active; }
		static int32_t padding;

	private:
		bool m_active = false;
		RootContainer m_container;
	};
}