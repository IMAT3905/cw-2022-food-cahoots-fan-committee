/* \file UILayer.cpp */
#include "UILayer.h"

UILayer::UILayer(const char* name) : Layer(name)
{
	auto& window = Application::getInstance().getWindow();

	Renderer2D::init();
	projection2D = glm::ortho(0.f, static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()), 0.f);

	m_swu["u_view"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(view2D)));
	m_swu["u_projection"] = std::pair<ShaderDataType, void*>(ShaderDataType::Mat4, static_cast<void*>(glm::value_ptr(projection2D)));

	HorizontalContainer top, middle, bottom;
	top.AddWidget<Spacer>(100, 10);
	top.AddWidget<Label>(300, 100, "Hello World!");

	middle.AddWidget<Spacer>(50, 10);
	middle.AddWidget<Label>(500, 100, "This is some text!");

	bottom.AddWidget<Button>(200, 60, "Button 1", []() {Log::info("I clicked on a button"); });

	m_window.AddContainer(top);
	m_window.AddContainer(middle);
	m_window.AddContainer(bottom);
	m_window.show();
}

void UILayer::onRender()
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

void UILayer::onKeyPressed(KeyPressedEvent& e) {
	Log::debug("Key Pressed");
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

void UILayer::onMouseMoved(MouseMovedEvent& e) {
	glm::ivec2 mousepos = e.getPos();
	m_window.OnMouseMove(mousepos);
}

void UILayer::onMousePressed(MouseButtonPressedEvent& e) {
	Log::debug("Mouse Pressed");
	glm::ivec2 mousepos = InputPoller::getMousePosition();
	m_window.OnMousePress(mousepos, e.getButton());
}

void UILayer::onMouseReleased(MouseButtonReleasedEvent& e) {
	glm::ivec2 mousepos = InputPoller::getMousePosition();
	m_window.OnMousePress(mousepos, e.getButton());
}

void UILayer::onUpdate(float timestep)
{
	//Log::debug("This is being run every frame");
}