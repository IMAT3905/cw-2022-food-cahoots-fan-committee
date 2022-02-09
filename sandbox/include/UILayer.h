/* \file UILayer.h */

//Start
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "engine.h"
#include "rendering/renderer2D.h"

#include <box2d/box2d.h>

using namespace Engine;

//New enum state
enum class UILayerState {InActive, Activating, Active, Deactivating};

class UILayer : public Layer
{
public:
	UILayer(const char* name);
	void onRender() override;
	void onUpdate(float timestep) override; //!< Runs every frame
	UILayerState GetState() { return m_state; } //Return state
	b2World* world;
	b2Body* groundBody;
	b2Body* body;
private:
	//FixedOthroCameraController2D
	glm::mat4 view2D = glm::mat4(1.f);
	glm::mat4 projection2D;
	SceneWideUniforms m_swu;
	std::shared_ptr<Window> m_window; //Window to render on
	UILayerState m_state = UILayerState::InActive; //Default state
	std::vector<Quad> m_quads;
	float advance;
};