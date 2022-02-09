#include "movementScript.h"

MovementScript::MovementScript(entt::entity entity) : m_registry(Engine::Application::getInstance().getRegistry()), NativeScript(entity)
{
	
}

glm::vec3 MovementScript::position(float t)
{

	return glm::vec3();
}
