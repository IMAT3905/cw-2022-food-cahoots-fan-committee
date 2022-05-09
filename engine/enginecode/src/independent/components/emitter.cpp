#include "engine_pch.h"
#include <entt/entt.hpp>
#include "core/application.h"
#include "components/emitter.h"
#include "components/transform.h"

namespace Engine
{
	/*namespace EmitterSystem
	{
		void update(float timestep)
		{
			entt::registry& registry = Application::getInstance().m_registry;

			auto view = registry.view<EmitterComponent>();

			for (auto entity : view)
			{
				auto& [emitter, t] = registry.get<EmitterComponent, TransformComponent>(entity);
				auto& transform = t.getTransform();


				glm::mat4 rotation = transform;
				rotation[3] = { 0.f, 0.f, 0.f, 1.f };
				glm::mat4 translation = glm::mat4(1.f);
				translation[3] = transform[3];
				glm::vec4 posDelta(emitter.positionOffset, 0.f);
				translation[3] += posDelta;

				glm::mat4 particleTransform = translation * rotation;

				emitter.deviceProps.linearPosition = particleTransform[3];

				emitter.elapsedTime += timestep;

				while (emitter.elapsedTime > emitter.timeBetweenEmissions)
				{
					//Emit particle
					Particle p(emitter.hostProps, emitter.deviceProps, emitter.blendMode);
					ParticleSystem::addParticle(p);
					//reduce elapsed time by time between emissions
					emitter.elapsedTime -= emitter.timeBetweenEmissions;
				}
			}
		}
	}*/
}