#pragma once
#include "rendering/particleSystem.h"

namespace Engine
{
	class EmitterComponent
	{
	public:
		EmitterComponent()
		{
			emissionRate = 10.f;
			timeBetweenEmissions = 1.0f / emissionRate;
		}

		EmitterComponent(float rate, glm::vec3& entityPosition, glm::vec3& p_positionOffset) : positionOffset(p_positionOffset)
		{
			if (rate <= 0) emissionRate = 1.0f;
			else emissionRate = rate;

			timeBetweenEmissions = 1.0f / emissionRate;

			deviceProps.linearPosition = entityPosition + positionOffset;
		}

		ParticleHostProprties hostProps;
		particleDeviceProperties deviceProps;
		BlendModes blendMode = BlendModes::None;
		glm::vec3 positionOffset;
		float emissionRate;
		float timeBetweenEmissions;
		float elapsedTime = 0.f;
		//bool trackingVelocity = false;
	};

	namespace EmitterSystem
	{
		void update(float timestep);
	}
}