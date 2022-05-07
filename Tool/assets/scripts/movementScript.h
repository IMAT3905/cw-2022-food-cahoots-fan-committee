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
	MovementScript(entt::entity& entity, float startT) : NativeScript(entity), t(startT) {};
	virtual void onCreate() override;
	virtual void onUpdate(float timestep) override;
	glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t);

private:
	entt::registry& m_registry = Engine::Application::getInstance().getRegistry();
	float speed = 0.1;
	MovementKeyFrames keyFrames[9];
	glm::vec3 pos;
	float t = 0.f;
	float height = 5.f;
	float width = 5.f;
	float radius = 0.5f;
	bool first = true;
	float angle = 0.f;
	glm::vec3 positions[8] =
	{
		glm::vec3(2.5f, 0.f, 3.f),
		glm::vec3(2.5f - width, 0.f, 3.f),
		glm::vec3(2.5f - width - radius, 0.f, 3.f - radius),
		glm::vec3(2.5f - width - radius, 0.f, 3.f - radius - height),
		glm::vec3(2.5f - width, 0.f, 3.f - radius - height - radius),
		glm::vec3(2.5f, 0.f, 3.f - radius - height - radius),
		glm::vec3(2.5f + radius, 0.f, 3.f - height - radius),
		glm::vec3(2.5f + radius, 0.f, 3.f - radius),
	};
};