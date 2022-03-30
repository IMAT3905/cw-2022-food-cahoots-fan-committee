#pragma once

/* \file MenuLayer.h */

//Start
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "engine.h"
#include "rendering/renderer2D.h"
#include "include/independent/UI/ModalWindow.h"


using namespace Engine;


class MenuLayer : public Layer
{
public:
	MenuLayer(const char* name);
	void onRender() override;
	void onUpdate(float timestep) override; //!< Runs every frame
	enum class UILayerState { InActive, Activating, Active, Deactivating };
	UILayerState GetState() { return m_state; } //Return state

	virtual void onKeyPressed(KeyPressedEvent& e) override;
	virtual void onMouseMoved(MouseMovedEvent& e) override;
	virtual void onMousePressed(MouseButtonPressedEvent& e) override;
	virtual void onMouseReleased(MouseButtonReleasedEvent& e) override;
	void SetMenu();
	void ButtonCall();
	UILayerState m_state = UILayerState::InActive; //Default state
private:
	//FixedOthroCameraController2D
	glm::mat4 view2D = glm::mat4(1.f);
	glm::mat4 projection2D;
	SceneWideUniforms m_swu;
	ModalWindow m_window; //Window to render on
};