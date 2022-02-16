/* \file engineApp.cpp */
#include "engineApp.h"

EngineApp::EngineApp(Engine::ApplicationProps props) : Application(props)
{
	//m_layerStack.push(new SceneLayer("Scene Layer"));
	m_layerStack.push(new UILayer("UI Layer"));
}

EngineApp::~EngineApp()
{

}

EngineApp::Application* Engine::startApplication()
{
	ApplicationProps props;
	props.winProps.title = "Sandbox App";
	props.winProps.width = 1024;
	props.winProps.height = 800;
	props.fontFilePath = "./assets/fonts/CURLZ___.TTF";
	props.characterSize = 56;

	return new EngineApp(props);
}