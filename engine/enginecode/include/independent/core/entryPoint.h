/* \file entryPoint.h */
#pragma once

#include "core/application.h"

extern Engine::Application* Engine::startApplication(); //!< Used to start the application

int main(int argc, char** argv)
{
	auto application = Engine::startApplication();
	application->run();
	delete application;

	return 0;
}

