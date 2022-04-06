#pragma once

#include "scripting.h"

namespace Engine{
	class PlayerScript : public NativeScript {
	public:
		PlayerScript(entt::entity& entity) : m_entity(entity) {};
		void onCreate() override;
		void onDestroy() override;
		void onUpdate(float timestep) override;
		void onKeyPress(KeyPressedEvent& e) override;
	protected:
		entt::entity m_entity;
	};
}