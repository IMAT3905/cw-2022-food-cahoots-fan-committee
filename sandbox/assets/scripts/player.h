#pragma once

#include "engine.h"
#include "include/independent/components/scripting.h"
#include "include/independent/events/codes.h"
namespace Engine {
	class PlayerScript : public Engine::NativeScript
	{
	public:
		PlayerScript(entt::entity& entity, int newkey) : m_entity(entity), key(newkey) {};
		virtual void onCreate() override;
		virtual void onDestroy() override;
		virtual void onUpdate(float timestep) override;
		virtual void onKeyPress(KeyPressedEvent& e) override;

		int score = 0;
		int key = NG_KEY_0;
	protected:
		entt::entity m_entity;
	};
}