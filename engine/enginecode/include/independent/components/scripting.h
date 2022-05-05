/* \file scripting.h */
#pragma once

#include <entt/entt.hpp>
#include "events/events.h"

namespace Engine
{
	class NativeScript
	{
	public:
		NativeScript() : m_entity(entt::null) {};
		NativeScript(entt::entity& entity) : m_entity(entity) {};
		virtual ~NativeScript() = default;
		virtual void onCreate() {};
		virtual void onDestroy() {};
		virtual void onUpdate(float timestep) {};
		virtual void onKeyPress(KeyPressedEvent& e) {};
	protected:
		entt::entity m_entity;
	};

	class NativeScriptComponent
	{
	public:
		NativeScriptComponent() = default;
		~NativeScriptComponent() = default;
		void onUpdate(float timestep) { m_instance->onUpdate(timestep); };
		void onKeyPress(KeyPressedEvent& e) { m_instance->onKeyPress(e); };
		template <typename G, typename ...Args> void create(Args&& ... args)
		{
			G* ptr = new G(std::forward<Args>(args) ...);
			m_instance.reset(static_cast<NativeScript*>(ptr));
			m_instance->onCreate();
		}

		int score = 0;
	protected:
		std::shared_ptr<NativeScript> m_instance = nullptr;
	};

	namespace ScriptSystem
	{
		void updateScripts(float timestep);
		void onKeyPressed(KeyPressedEvent& e);
	}
}