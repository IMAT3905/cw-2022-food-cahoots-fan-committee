/* \file movementScript.h */
#pragma once

#include "engine.h"

struct MovementKeyFrames
{
	float t;

};

class MovementScript : public Engine::NativeScript
{
public:
	MovementScript(entt::entity entity);

	void onUpdate(float timestep) override
	{
		elapsedTime += timestep;
		if (elapsedTime > 1.f)
		{
			auto& tc = m_registry.get<Engine::TransformComponent>(m_entity);
			elapsedTime = 0.f;
			if (toggle)
			{
				tc.translation = glm::vec3(-2.f, 1.f, -4.f);
				tc.updateTransform();
			}
			else
			{
				tc.translation = glm::vec3(2.f, 1.f, -6.f);
				tc.updateTransform();
			}
			toggle = !toggle;
		}
	}
private:
	entt::registry& m_registry;
	float elapsedTime = 0.f;
	bool toggle = false;
	float r = 0.7f;
	float edgeWidth = 5.f;
	float edgeHeight = 5.f;
	float speed = 0.01;
	MovementKeyFrames keyFrames[8];
	glm::vec3 position(float t);
	//position[0] = 
};