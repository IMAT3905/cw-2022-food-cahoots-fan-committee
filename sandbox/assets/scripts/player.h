#pragma once

#include "engine.h"
#include "include/independent/components/scripting.h"
namespace Engine {
	class PlayerScript : public Engine::NativeScript
	{
	public:
		PlayerScript(entt::entity& entity) : m_entity(entity) {};
		virtual void onCreate() override;
		virtual void onDestroy() override;
		virtual void onUpdate(float timestep) override;
		virtual void onKeyPress(KeyPressedEvent& e) override;

		int scores[4] = { 0,0,0,0 };
	protected:
		entt::entity m_entity;
	};
}