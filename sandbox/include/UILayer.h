/* \file UILayer.h */

//Start
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "engine.h"
#include "rendering/renderer2D.h"
#include "include/independent/UI/ModalWindow.h"



using namespace Engine;

//New enum state
enum class UILayerState {InActive, Activating, Active, Deactivating};
enum class UIMode {MainMenu, InGame};

class UILayer : public Layer
{
public:
	UILayer(const char* name);
	void onRender() override;
	void onUpdate(float timestep) override; //!< Runs every frame
	UILayerState GetState() { return m_state; } //Return state

	virtual void onKeyPressed(KeyPressedEvent& e) override;
	virtual void onMouseMoved(MouseMovedEvent& e) override;
	virtual void onMousePressed(MouseButtonPressedEvent& e) override;
	virtual void onMouseReleased(MouseButtonReleasedEvent& e) override;
	void SetMenu();
	void SetInGame();

private:
	//FixedOthroCameraController2D
	glm::mat4 view2D = glm::mat4(1.f);
	glm::mat4 projection2D;
	SceneWideUniforms m_swu;
	ModalWindow m_window; //Window to render on
	UILayerState m_state = UILayerState::InActive; //Default state
	UIMode m_mode = UIMode::InGame;
};