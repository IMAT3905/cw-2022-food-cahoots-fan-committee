/* \file movementScript.h */
#pragma once

#include "engine.h"

class MovementScript : public Engine::NativeScript
{
public:
	MovementScript() : m_registry(Engine::Application::getInstance().getRegistry()) {}

	void onUpdate(float timestep) override
	{
		glm::vec2 desiredVel(0.f);
		if (Engine::InputPoller::isKeyPressed(NG_KEY_LEFT))
		{
			desiredVel.x = 2.5f;
		}

		if (Engine::InputPoller::isKeyPressed(NG_KEY_RIGHT))
		{
			desiredVel.x += 2.5f;
		}

		if (desiredVel.x != 0.f)
		{
			glm::vec2 force(0.f);

			float mass;
		}
	}
private:
	entt::registry& m_registry;
};