#pragma once

#include "engine.h"
#include "include/independent/components/scripting.h"
#include "include/independent/events/codes.h"
#include "imGuiLayer.h"
namespace Engine {
	class PlayerScript : public Engine::NativeScript
	{
	public:
		PlayerScript(entt::entity& entity, int newkey, int newid, ImGuiLayer& newscene) : m_entity(entity), key(newkey), id(newid), scene(newscene) {};
		virtual void onCreate() override;
		virtual void onDestroy() override;
		virtual void onUpdate(float timestep) override;
		virtual void onKeyPress(KeyPressedEvent& e) override;

		int score = 0;
		int key = NG_KEY_0;
	protected:
		entt::entity m_entity;
		ImGuiLayer& scene;
		int id; //Array ID of player, used for updating score in SceneLayer
	};
}