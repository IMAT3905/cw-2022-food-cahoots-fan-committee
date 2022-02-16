/* \file movementScript.h */
#pragma once

#include "engine.h"

struct MovementKeyFrames
{
	/*float t;
	float height = 5.f;
	float width = 5.f;
	float radius = 0.5f;
	glm::vec3 position[8] =
	{
		glm::vec3(0 - width, 0, 0),
		glm::vec3(0 - width - radius, 0, 0 + radius),
		glm::vec3(0 - width - radius, 0, 0 + radius + height),
		glm::vec3(0 - width - radius + radius, 0, 0 + radius + height + radius),
		glm::vec3(0 - width - radius + radius + width, 0, 0 + radius + height + radius),
		glm::vec3(0 - width - radius + radius + width + radius, 0, 0 + radius + height + radius - radius),
		glm::vec3(0 - width - radius + radius + width + radius, 0, 0 + radius + height + radius - radius - height),
		glm::vec3(0 - width - radius + radius + width + radius - radius, 0, 0 + radius + height + radius - radius - height - radius)
	};*/
};

class MovementScript : public Engine::NativeScript
{
public:
	MovementScript(entt::entity& entity) : NativeScript(entity) {};

	virtual void onCreate() override
	{
	};

	virtual void onUpdate(float timestep) override
	{
		entt::registry& m_registry = Application::getInstance().getRegistry();
		elapsedTime += timestep;	
		auto& tc = m_registry.get<Engine::TransformComponent>(m_entity);
		int roundedTime = elapsedTime;
		tc.translation = position[roundedTime];
		tc.updateTransform();
		if (elapsedTime > 8.f)
		{
			elapsedTime = 0;
		}
		/*if (elapsedTime > 1.f)
		{
			auto& tc = m_registry.get<Engine::TransformComponent>(m_entity);
			elapsedTime = 0.f;
			if (toggle)
			{
				tc.translation = glm::vec3(0.f, 0.f, 0.f);
				tc.updateTransform();
			}
			else
			{
				tc.translation = glm::vec3(-5.f, 0.f, -5.f);
				tc.updateTransform();
			}
			toggle = !toggle;
		}*/
		
	}
private:
	//entt::registry& m_registry;
	float elapsedTime = 0.f;
	bool toggle = false;
	float r = 0.7f;
	float edgeWidth = 5.f;
	float edgeHeight = 5.f;
	float speed = 0.01;
	MovementKeyFrames keyFrames[8];
	//glm::vec3 position(float t);

	float t;
	float height = 5.f;
	float width = 5.f;
	float radius = 0.5f;
	glm::vec3 position[8] =
	{
		glm::vec3(0.f - width, 0, -0.f),
		glm::vec3(0.f - width - radius, 0, -0.f + radius),
		glm::vec3(0.f - width - radius, 0, -0.f + radius + height),
		glm::vec3(0.f - width - radius + radius, 0, -0.f + radius + height + radius),
		glm::vec3(0.f - width - radius + radius + width, 0, -0.f + radius + height + radius),
		glm::vec3(0.f - width - radius + radius + width + radius, 0, -0.f + radius + height + radius - radius),
		glm::vec3(0.f - width - radius + radius + width + radius, 0, -0.f + radius + height + radius - radius - height),
		glm::vec3(0.f - width - radius + radius + width + radius - radius, 0, -0.f + radius + height + radius - radius - height - radius)
	};
};