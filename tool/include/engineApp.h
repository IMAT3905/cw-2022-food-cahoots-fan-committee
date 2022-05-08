/* \file engineApp.h */
#pragma once

//entry point
#include "include/independent/core/entryPoint.h"
#include "engine.h"
#include "InGameLayer.h"
#include "MenuLayer.h"
#include "imGuiLayer.h"

class EngineApp : public Engine::Application
{
public:
	EngineApp(Engine::ApplicationProps props);
	~EngineApp() override;
private:
	std::shared_ptr<MenuLayer> m_menulayer;
	std::shared_ptr<InGameLayer> m_ingamelayer;
	std::shared_ptr<ImGuiLayer> m_sceneLayer;
};