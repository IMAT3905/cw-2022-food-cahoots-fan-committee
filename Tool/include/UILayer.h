/* \file UILayer.h */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "engine.h"
#include "rendering/renderer2D.h"

#include <box2d/box2d.h>

using namespace Engine;

class UILayer : public Layer
{
public:
	UILayer(const char* name);
	void onRender() override;
	void onUpdate(float timestep) override; //!< Runs every frame
	b2World* world;
	b2Body* groundBody;
	b2Body* body;
private:
	glm::mat4 view2D = glm::mat4(1.f);
	glm::mat4 projection2D;
	SceneWideUniforms m_swu;
	std::vector<Quad> m_quads;
	float advance;
};