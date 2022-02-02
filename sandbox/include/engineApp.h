/* \file engineApp.h */
#pragma once

//entry point
#include "include/independent/core/entryPoint.h"
#include "engine.h"
#include "UILayer.h"
#include "sceneLayer.h"

class EngineApp : public Engine::Application
{
public:
	EngineApp(Engine::ApplicationProps props);
	~EngineApp() override;
private:
	std::shared_ptr<UILayer> m_uiLayer;
	std::shared_ptr<SceneLayer> m_sceneLayer;
};