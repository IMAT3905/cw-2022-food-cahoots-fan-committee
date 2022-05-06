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

	m_isFocused = false;
	m_isDisplayed = false;
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
	case NG_KEY_1:
		scores[0] += 1;
		break;
	case NG_KEY_2:
		scores[1] += 1;
		break;
	case NG_KEY_3:
		scores[2] += 1;
		break;
	case NG_KEY_4:
		scores[3] += 1;
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
	m_scoreStr[0] = ": " + std::to_string(scores[0]);
	m_scoreStr[1] = ": " + std::to_string(scores[1]);
	m_scoreStr[2] = ": " + std::to_string(scores[2]);
	m_scoreStr[3] = ": " + std::to_string(scores[3]);
	currenttime = "Timer: " + std::to_string(selecttime);
}

void InGameLayer::SetInGame() {
	HorizontalContainer top, bottom;
	top.AddWidget<Spacer>(250, 500);
	top.AddWidget<Label>(300, 100, currenttime.c_str(), Justification::left);
	top.AddWidget<Spacer>(0, 650);

	bottom.AddWidget<Spacer>(0, 0);
	bottom.AddWidget<ColouredSquare>(50, 50, glm::vec4(1, 0, 0, 1));
	bottom.AddWidget<Spacer>(10, 0);
	bottom.AddWidget<Label>(10, 100, m_scoreStr[0].c_str(), Justification::left);

	bottom.AddWidget<Spacer>(200, 0);
	bottom.AddWidget<ColouredSquare>(50, 50, glm::vec4(0, 1, 0, 1));
	bottom.AddWidget<Spacer>(10, 0);
	bottom.AddWidget<Label>(10, 100, m_scoreStr[1].c_str(), Justification::left);

	bottom.AddWidget<Spacer>(200, 0);
	bottom.AddWidget<ColouredSquare>(50, 50, glm::vec4(0, 0, 1, 1));
	bottom.AddWidget<Spacer>(10, 0);
	bottom.AddWidget<Label>(10, 100, m_scoreStr[2].c_str(), Justification::left);

	bottom.AddWidget<Spacer>(200, 0);
	bottom.AddWidget<ColouredSquare>(50, 50, glm::vec4(1, 0, 1, 1));
	bottom.AddWidget<Spacer>(10, 0);
	bottom.AddWidget<Label>(10, 100, m_scoreStr[3].c_str(), Justification::left);

	m_window.AddContainer(top);
	m_window.AddContainer(bottom);
}

void InGameLayer::ButtonCall() {

}