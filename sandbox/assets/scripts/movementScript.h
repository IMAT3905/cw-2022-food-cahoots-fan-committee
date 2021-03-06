/* \file movementScript.h */
#pragma once

#include "engine.h"

struct MovementKeyFrames
{
	float t;
	glm::vec3 position;
};

class MovementScript : public Engine::NativeScript
{
public:
	MovementScript(entt::entity& entity, float startT, float nypos) : NativeScript(entity), t(startT) , ypos(nypos) {};
	virtual void onCreate() override;
	virtual void onUpdate(float timestep) override;
	glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t);

private:
	entt::registry& m_registry = Engine::Application::getInstance().getRegistry();
	float speed = 0.3; //0.3
	MovementKeyFrames keyFrames[17];
	glm::vec3 pos;
	float t = 0.f;
	float height = 5.f;
	float width = 5.f;
	float radius = 0.5f;
	bool first = true;
	float angle = 0.f;
	float ypos = 0.f;
	glm::vec3 positions[16] =
	{
		glm::vec3(2.5f, ypos, 3.f),
		glm::vec3(2.5f - (width / 3.f), ypos, 3.f),
		glm::vec3(2.5f - width / 3.f * 2.f, ypos, 3.f),
		glm::vec3(2.5f - width, ypos, 3.f),
		glm::vec3(2.5f - width - radius, ypos, 3.f - radius),
		glm::vec3(2.5f - width - radius, ypos, 3.f - radius - height / 3.f),
		glm::vec3(2.5f - width - radius, ypos, 3.f - radius - height / 3.f * 2.f),
		glm::vec3(2.5f - width - radius, ypos, 3.f - radius - height),
		glm::vec3(2.5f - width, ypos, 3.f - radius - height - radius),
		glm::vec3(2.5f - width + (width / 3.f), ypos, 3.f - radius - height - radius),
		glm::vec3(2.5f - width + (width / 3.f * 2.f), ypos, 3.f - radius - height - radius),
		glm::vec3(2.5f, ypos, 3.f - radius - height - radius),
		glm::vec3(2.5f + radius, ypos, 3.f - height - radius),
		glm::vec3(2.5f + radius, ypos, 3.f - height + (height / 3.f) - radius),
		glm::vec3(2.5f + radius, ypos, 3.f - height + (height / 3.f * 2.f) - radius),
		glm::vec3(2.5f + radius, ypos, 3.f - radius),
	};
};