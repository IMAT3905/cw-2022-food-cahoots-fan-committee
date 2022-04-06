#pragma once

#include "engine.h"
#include "scripting.h"

class PlayerScript : public Engine::NativeScript
{
public:
	PlayerScript(entt::entity& entity, float startT) : NativeScript(entity), t(startT) {};
	virtual void onCreate() override;
	virtual void onDestroy() override;
	virtual void onUpdate(float timestep) override;
	virtual void onKeyPress(KeyPressedEvent& e) override;
protected:
	entt::entity m_entity;
};