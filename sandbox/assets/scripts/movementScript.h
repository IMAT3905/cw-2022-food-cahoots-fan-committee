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
	MovementScript(entt::entity& entity, uint32_t startPoint) : NativeScript(entity), startPoint(startPoint) {};

	virtual void onCreate() override
	{
		float l = (2.f * (width + height)) + (2.f * glm::pi<float>() * radius);

		keyFrames[0].t = 0.f;
		keyFrames[0].position = positions[0];

		keyFrames[1].t = width;
		keyFrames[1].position = positions[1];

		keyFrames[2].t = keyFrames[1].t + (glm::pi<float>() * radius) / 2.f;
		keyFrames[2].position = positions[2];

		keyFrames[3].t = keyFrames[2].t + height;
		keyFrames[3].position = positions[3];

		keyFrames[4].t = keyFrames[3].t + (glm::pi<float>() * radius) / 2.f;
		keyFrames[4].position = positions[4];

		keyFrames[5].t = keyFrames[4].t + width;
		keyFrames[5].position = positions[5];

		keyFrames[6].t = keyFrames[5].t + (glm::pi<float>() * radius) / 2.f;
		keyFrames[6].position = positions[6];

		keyFrames[7].t = keyFrames[6].t + height;
		keyFrames[7].position = positions[7];

		keyFrames[8].t = l;
		keyFrames[8].position = positions[0];

		for (size_t i = 0; i < 9; i++) keyFrames[i].t /= l;

		if (startPoint > 8) startPoint = 0;
		auto& tc = m_registry.get<Engine::TransformComponent>(m_entity);
		tc.translation = keyFrames[startPoint].position;
		tc.updateTransform();
	}

	glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t)
	{
		glm::vec3 position = a + t * (b - a);
		return position;
	}

	virtual void onUpdate(float timestep) override
	{
		elapsedTime += timestep;
		t += speed * (timestep * 0.5);
		if (t > 1.0f) t -= 1.f;
		//angle = 0.f;
		for (size_t i = 0; i < 9; i++)
		{
			/*if (first)
			{
				i += startPoint;
				first = false;
			}*/
			
			if (keyFrames[i].t <= t && t <= keyFrames[i + 1].t)
			{
				float local_t = (t - keyFrames[i].t) / (keyFrames[i + 1].t - keyFrames[i].t); //(t-s)/(e-s)
				if (i == 1)
				{
					pos = keyFrames[i].position;
					glm::vec3 c(0.f);
					c.x = keyFrames[1].position.x;
					c.z = keyFrames[2].position.z;
					float halfPi = glm::pi<float>() * 0.5f;
					float theta = halfPi + (local_t * halfPi);

					pos.x = c.x + cosf(theta) * radius;
					pos.z = c.z + sinf(theta) * radius;

					angle = -(theta + halfPi) + (halfPi * 2.f);
					Engine::Log::debug("theta: {0}", theta);
				}
				else if (i == 3)
				{
					pos = keyFrames[i].position;
					glm::vec3 c(0.f);
					c.x = keyFrames[4].position.x;
					c.z = keyFrames[3].position.z;
					float halfPi = glm::pi<float>() * 0.5f;
					float theta = (2.f * halfPi) + (local_t * halfPi);

					pos.x = c.x + cosf(theta) * radius;
					pos.z = c.z + sinf(theta) * radius;

					angle = -(theta + halfPi) + (halfPi * 2.f);
					Engine::Log::debug("theta: {0}", theta);
				}
				else if (i == 5)
				{
					pos = keyFrames[i].position;
					glm::vec3 c(0.f);
					c.x = keyFrames[5].position.x;
					c.z = keyFrames[6].position.z;
					float halfPi = glm::pi<float>() * 0.5f;
					float theta = (3.f * halfPi) + (local_t * halfPi);

					pos.x = c.x + cosf(theta) * radius;
					pos.z = c.z + sinf(theta) * radius;

					angle = -(theta + halfPi) + (halfPi * 2.f);
					Engine::Log::debug("theta: {0}", theta);
				}
				else if (i == 7)
				{
					pos = keyFrames[i].position;
					glm::vec3 c(0.f);
					c.x = keyFrames[8].position.x;
					c.z = keyFrames[7].position.z;
					float halfPi = glm::pi<float>() * 0.5f;
					float theta = (4.f * halfPi) + (local_t * halfPi);

					pos.x = c.x + cosf(theta) * radius;
					pos.z = c.z + sinf(theta) * radius;

					angle = -(theta + halfPi) + (halfPi * 2.f);
					Engine::Log::debug("theta: {0}", theta);
				}
				else
				{
					pos = lerp(keyFrames[i].position, keyFrames[i + 1].position, local_t);
				}
				
				//pos = keyFrames[i].position;
				
			}
		}
		auto& tc = m_registry.get<Engine::TransformComponent>(m_entity);
		tc.translation = pos;
		tc.setRotation(glm::vec3(0.0, angle, 0.0));
		tc.updateTransform();
	}

private:
	entt::registry& m_registry = Engine::Application::getInstance().getRegistry();
	float elapsedTime = 0.f;
	bool toggle = false;
	float speed = 0.1;
	MovementKeyFrames keyFrames[9];
	glm::vec3 pos;
	float t = 0.f;
	float height = 5.f;
	float width = 5.f;
	float radius = 0.5f;
	uint32_t startPoint;
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
		glm::vec3(2.5f + radius, 0.f, 3.f - radius - height),
		glm::vec3(2.5f + radius, 0.f, 3.f - radius),
	};
};