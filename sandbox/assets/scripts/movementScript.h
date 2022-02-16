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
	MovementScript(entt::entity& entity) : NativeScript(entity) {};

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

		keyFrames[6].t = keyFrames[4].t + (glm::pi<float>() * radius) / 2.f;
		keyFrames[6].position = positions[6];

		keyFrames[7].t = l;
		keyFrames[7].position = positions[7];

		for (size_t i = 0; i < 8; i++) keyFrames[i].t /= l;
	};

	glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t)
	{
		glm::vec3 position = a + t * (b - a);
		return position;
	};

	virtual void onUpdate(float timestep) override
	{
		entt::registry& m_registry = Application::getInstance().getRegistry();
		elapsedTime += timestep;	
		t += speed * timestep;
		if (t > 1.0f) t -= 1.f;
		glm::vec3 pos;
		
		bool hit = false;
		for (size_t i = 0; i < 7; i++)
		{
			if (keyFrames[i].t <= t && t <= keyFrames[i+1].t)
			{
				float local_t = (t - keyFrames[i].t) / (keyFrames[i + 1].t - keyFrames[i].t);//(t-s)/(e-s)
				Log::debug("local t {0}", local_t);
				pos = keyFrames[i].position;
				pos = lerp(keyFrames[i].position, keyFrames[i + 1].position, local_t);
				hit = true;
				if (local_t > 1.0f)
				{
					int a = 0;
				}
			}
		}

		if (!hit)
		{
			int a = 0;
		}
		auto& tc = m_registry.get<Engine::TransformComponent>(m_entity);

		tc.translation = pos;
		tc.updateTransform();
		/*if (tc.translation != position[nextPosition])
		{
			tc.translation = lerp(tc.translation, position[nextPosition], speed);
			tc.updateTransform();
		}	*/	
	}

private:
	//entt::registry& m_registry;
	float elapsedTime = 0.f;
	bool toggle = false;
	float r = 0.7f;
	float edgeWidth = 5.f;
	float edgeHeight = 5.f;
	float speed = 0.1;
	MovementKeyFrames keyFrames[8];
	//glm::vec3 position(float t);

	float t = 0.f;
	float height = 5.f;
	float width = 5.f;
	float radius = 0.5f;
	glm::vec3 positions[8] =
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