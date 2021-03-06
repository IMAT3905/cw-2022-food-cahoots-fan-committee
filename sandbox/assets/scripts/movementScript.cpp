#include "movementScript.h"

//MovementScript::MovementScript(entt::entity& entity, uint32_t startPoint) : NativeScript(entity), startPoint(startPoint) {};

void MovementScript::onCreate()
{ 
	float l = (2.f * (width + height)) + (2.f * glm::pi<float>() * radius);

	keyFrames[0].t = 0.f;
	keyFrames[0].position = positions[0];

	keyFrames[1].t = width / 3.f;
	keyFrames[1].position = positions[1];

	keyFrames[2].t = width / 3.f * 2.f;
	keyFrames[2].position = positions[2];

	keyFrames[3].t = width;
	keyFrames[3].position = positions[3];

	keyFrames[4].t = keyFrames[3].t + (glm::pi<float>() * radius) / 2.f;
	keyFrames[4].position = positions[4];

	keyFrames[5].t = keyFrames[4].t + height / 3.f;
	keyFrames[5].position = positions[5];

	keyFrames[6].t = keyFrames[4].t + height / 3.f * 2.f;
	keyFrames[6].position = positions[6];

	keyFrames[7].t = keyFrames[4].t + height;
	keyFrames[7].position = positions[7];

	keyFrames[8].t = keyFrames[7].t + (glm::pi<float>() * radius) / 2.f;
	keyFrames[8].position = positions[8];

	keyFrames[9].t = keyFrames[8].t + width / 3.f;
	keyFrames[9].position = positions[9];

	keyFrames[10].t = keyFrames[8].t + width / 3.f * 2.f;
	keyFrames[10].position = positions[10];

	keyFrames[11].t = keyFrames[8].t + width;
	keyFrames[11].position = positions[11];

	keyFrames[12].t = keyFrames[11].t + (glm::pi<float>() * radius) / 2.f;
	keyFrames[12].position = positions[12];

	keyFrames[13].t = keyFrames[12].t + height / 3.f;
	keyFrames[13].position = positions[13];

	keyFrames[14].t = keyFrames[12].t + height / 3.f * 2.f;
	keyFrames[14].position = positions[14];

	keyFrames[15].t = keyFrames[12].t + height;
	keyFrames[15].position = positions[15];

	keyFrames[16].t = l;
	keyFrames[16].position = positions[0];

	for (size_t i = 0; i < 17; i++) keyFrames[i].t /= l;
};

void MovementScript::onUpdate(float timestep)
{
	t += speed * (timestep / 4);
	if (t > 1.0f) t -= 1.f;
	//Engine::Log::debug("t: {0}", t);
	for (size_t i = 0; i < 17; i++)
	{
		if (keyFrames[i].t <= t && t <= keyFrames[i + 1].t)
		{
			float local_t = (t - keyFrames[i].t) / (keyFrames[i + 1].t - keyFrames[i].t);
			if (i == 3)
			{
				pos = keyFrames[i].position;
				glm::vec3 c(0.f);
				c.x = keyFrames[3].position.x;
				c.z = keyFrames[4].position.z;
				float halfPi = glm::pi<float>() * 0.5f;
				float theta = halfPi + (local_t * halfPi);

				pos.x = c.x + cosf(theta) * radius;
				pos.z = c.z + sinf(theta) * radius;

				angle = -(theta + halfPi) + (halfPi * 2.f);
				//Engine::Log::debug("theta: {0}", theta);
			}
			else if (i == 7)
			{
				pos = keyFrames[i].position;
				glm::vec3 c(0.f);
				c.x = keyFrames[8].position.x;
				c.z = keyFrames[7].position.z;
				float halfPi = glm::pi<float>() * 0.5f;
				float theta = (2.f * halfPi) + (local_t * halfPi);

				pos.x = c.x + cosf(theta) * radius;
				pos.z = c.z + sinf(theta) * radius;

				angle = -(theta + halfPi) + (halfPi * 2.f);
				//Engine::Log::debug("theta: {0}", theta);
			}
			else if (i == 11)
			{
				pos = keyFrames[i].position;
				glm::vec3 c(0.f);
				c.x = keyFrames[11].position.x;
				c.z = keyFrames[12].position.z;
				float halfPi = glm::pi<float>() * 0.5f;
				float theta = (3.f * halfPi) + (local_t * halfPi);

				pos.x = c.x + cosf(theta) * radius;
				pos.z = c.z + sinf(theta) * radius;

				angle = -(theta + halfPi) + (halfPi * 2.f);
				//Engine::Log::debug("theta: {0}", theta);
			}
			else if (i == 15)
			{
				pos = keyFrames[i].position;
				glm::vec3 c(0.f);
				c.x = keyFrames[16].position.x;
				c.z = keyFrames[15].position.z;
				float halfPi = glm::pi<float>() * 0.5f;
				float theta = (4.f * halfPi) + (local_t * halfPi);

				pos.x = c.x + cosf(theta) * radius;
				pos.z = c.z + sinf(theta) * radius;

				angle = -(theta + halfPi) + (halfPi * 2.f);
				//Engine::Log::debug("theta: {0}", theta);
			}
			else
			{
				pos = lerp(keyFrames[i].position, keyFrames[i + 1].position, local_t);
			}
		}
	}
	auto& tc = m_registry.get<Engine::TransformComponent>(m_entity);
	tc.translation = pos;
	tc.setRotation(glm::vec3(0.0, angle, 0.0));
	tc.updateTransform();
};

glm::vec3 MovementScript::lerp(glm::vec3 a, glm::vec3 b, float t)
{
	glm::vec3 position = a + t * (b - a);
	return position;
};