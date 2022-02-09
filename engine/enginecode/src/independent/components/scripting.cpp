/* \file scripting.cpp */

#include "engine_pch.h"
#include "components/scripting.h"
#include "core/application.h"
#include <entt/entt.hpp>

namespace Engine
{
	void ScriptSystem::updateScripts(float timestep)
	{
		entt::registry& registry = Application::getInstance().getRegistry();

		auto scriptView = registry.view<NativeScriptComponent>();
		for (auto entity : scriptView)
		{
			auto& nsc = registry.get<NativeScriptComponent>(entity);
			nsc.onUpdate(timestep);
		}
	}

	void ScriptSystem::onKeyPressed(KeyPressedEvent& e)
	{
		entt::registry& registry = Application::getInstance().getRegistry();

		auto scriptView = registry.view<NativeScriptComponent>();
		for (auto entity : scriptView)
		{
			auto& nsc = registry.get<NativeScriptComponent>(entity);
			nsc.onKeyPress(e);
		}
	}
}