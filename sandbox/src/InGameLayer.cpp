/* \file UILayer.cpp */
#include "InGameLayer.h"

InGameLayer::InGameLayer(const char* name) : Layer(name)
{
	auto& window = Application::getInstance().getWindow();

	Renderer2D::init();
	projection2D = glm::ortho(0.f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.f);

	m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
	m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

	SetInGame();

	m_window.show();
}

void InGameLayer::onRender()
{
	switch (m_state) {
	case UILayerState::Activating:
		m_state = UILayerState::Active;
		break;
	case UILayerState::Deactivating:
		m_state = UILayerState::InActive;
		break;
	}

	if (m_state == UILayerState::Active) {
		RenderCommands::setDepthTestCommand(false)->action();
		RenderCommands::setBlendCommand(true)->action();
		RenderCommands::setTransparencyBlend()->action();
		Renderer2D::begin(m_swu);
		m_window.OnRender();
		Renderer2D::end();
	}
}

void InGameLayer::onKeyPressed(KeyPressedEvent& e) {
	switch (e.getKeyCode()) {
	case NG_KEY_G:
		m_state = UILayerState::Activating;
		Log::debug("Activating UI");
		break;
	case NG_KEY_H:
		m_state = UILayerState::Deactivating;
		Log::debug("Deactivating UI");
		break;
	}

	if (m_state == UILayerState::Active) e.handle(true);
}

void InGameLayer::onMouseMoved(MouseMovedEvent& e) {
	glm::ivec2 mousepos = e.getPos();
	m_window.OnMouseMove(mousepos);
}

void InGameLayer::onMousePressed(MouseButtonPressedEvent& e) {
	glm::ivec2 mousepos = InputPoller::getMousePosition();
	m_window.OnMousePress(mousepos, e.getButton());
}

void InGameLayer::onMouseReleased(MouseButtonReleasedEvent& e) {
	glm::ivec2 mousepos = InputPoller::getMousePosition();
	m_window.OnMousePress(mousepos, e.getButton());
}

void InGameLayer::onUpdate(float timestep)
{
	//Log::debug("This is being run every frame");
}

void InGameLayer::SetInGame() {
	HorizontalContainer top, bottom;

	top.AddWidget<Spacer>(250, 500);
	top.AddWidget<Label>(300, 100, "Timer: 10", Justification::left);
	top.AddWidget<Spacer>(0, 500);

	bottom.AddWidget<Spacer>(40, 0);
	bottom.AddWidget<ColouredSquare>(50, 50, glm::vec4(1, 0, 0, 1));
	bottom.AddWidget<Spacer>(10, 0);
	bottom.AddWidget<Label>(10, 100, ": 1", Justification::left);

	bottom.AddWidget<Spacer>(80, 0);
	bottom.AddWidget<ColouredSquare>(50, 50, glm::vec4(0, 1, 0, 1));
	bottom.AddWidget<Spacer>(10, 0);
	bottom.AddWidget<Label>(10, 100, ": 2", Justification::left);

	bottom.AddWidget<Spacer>(80, 0);
	bottom.AddWidget<ColouredSquare>(50, 50, glm::vec4(0, 0, 1, 1));
	bottom.AddWidget<Spacer>(10, 0);
	bottom.AddWidget<Label>(10, 100, ": 3", Justification::left);

	bottom.AddWidget<Spacer>(80, 0);
	bottom.AddWidget<ColouredSquare>(50, 50, glm::vec4(1, 0, 1, 1));
	bottom.AddWidget<Spacer>(10, 0);
	bottom.AddWidget<Label>(10, 100, ": 4", Justification::left);

	m_window.AddContainer(top);
	m_window.AddContainer(bottom);
}

void InGameLayer::ButtonCall() {

}