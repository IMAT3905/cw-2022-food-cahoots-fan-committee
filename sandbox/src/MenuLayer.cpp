/* \file UILayer.cpp */
#include "MenuLayer.h"

MenuLayer::MenuLayer(const char* name) : Layer(name)
{
	auto& window = Application::getInstance().getWindow();

	Renderer2D::init();
	projection2D = glm::ortho(0.f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.f);

	m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
	m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

	SetMenu();

	m_window.show();
}

void MenuLayer::onRender()
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

void MenuLayer::onKeyPressed(KeyPressedEvent& e) {
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

void MenuLayer::onMouseMoved(MouseMovedEvent& e) {
	glm::ivec2 mousepos = e.getPos();
	m_window.OnMouseMove(mousepos);
}

void MenuLayer::onMousePressed(MouseButtonPressedEvent& e) {
	glm::ivec2 mousepos = InputPoller::getMousePosition();
	m_window.OnMousePress(mousepos, e.getButton());
}

void MenuLayer::onMouseReleased(MouseButtonReleasedEvent& e) {
	glm::ivec2 mousepos = InputPoller::getMousePosition();
	m_window.OnMousePress(mousepos, e.getButton());
}

void MenuLayer::onUpdate(float timestep)
{
	//Log::debug("This is being run every frame");
}

void MenuLayer::SetMenu() {
	HorizontalContainer top, bottom;

	top.AddWidget<Spacer>(200, 500);
	top.AddWidget<Label>(300, 100, "Main Menu", Justification::left);

	bottom.AddWidget<Spacer>(200, 0);
	bottom.AddWidget<Button>(100, 100, "Start", [this]() {this->ButtonCall(); });

	m_window.AddContainer(top);
	m_window.AddContainer(bottom);
}

void MenuLayer::ButtonCall() {
	Log::info("Button Pressed");
	this->setDisplayed(false);
	this->setFocused(false);
}