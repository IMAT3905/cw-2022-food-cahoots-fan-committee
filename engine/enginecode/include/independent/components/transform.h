/* \file transform.h */
#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	class TransformComponent
	{
	public:
		glm::vec3 translation = { 0.f, 0.f, 0.f };
		glm::vec3 scale = { 1.f, 1.f, 1.f };

		TransformComponent() { transform = glm::mat4(1.0f); }
		TransformComponent(glm::vec3 t, glm::vec3 r, glm::vec3 s)
		{
			setTransform(t, r, s);
		}

		glm::mat4& setTransform(glm::vec3 t, glm::vec3 r, glm::vec3 s)
		{
			translation = t;
			rotation = glm::quat(r);
			scale = s;

			return updateTransform();
		};

		glm::mat4& setTransform(glm::mat4& parentTransform, glm::vec3 t, glm::vec3 r, glm::vec3 s)
		{
			translation = t;
			rotation = glm::quat(r);
			scale = s;

			return updateTransform(parentTransform);
		};

		glm::mat4& updateTransform()
		{
			/*glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), rotation.x, {1.f, 0.f, 0.f});
			glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), rotation.y, { 0.f, 1.f, 0.f });
			glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), rotation.z, { 0.f, 0.f, 1.f });
			glm::mat4 rot = rotX * rotY * rotZ;*/

			transform = glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), scale);

			return transform;
		}

		glm::mat4& updateTransform(glm::mat4& parentTransform)
		{
			transform = parentTransform * updateTransform();

			return transform;
		}

		glm::mat4& getTransform() { return transform; }

		void addRotation(glm::vec3& eulerAngles)
		{
			glm::quat rotationDelta(eulerAngles);
			rotation *= rotationDelta;
		}

		void setRotation(glm::vec3& eulerAngles)
		{
			rotation = glm::quat(eulerAngles);
		}
	private:
		glm::mat4 transform;
		glm::quat rotation;
	};
}