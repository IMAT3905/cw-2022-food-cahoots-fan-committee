/** \file renderCommands.cpp */

#include "engine_pch.h"
#include "rendering/RenderCommands.h"
#include "rendering/renderAPI.h"
#include <glad/glad.h>

#include "systems/log.h"
#include "platform/OpenGL/OpenGLCommands.h"

namespace Engine
{
	RenderCommands* RenderCommands::clearDepthAndColourBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLClearColourAndDepthBufferCommand();
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	RenderCommands* RenderCommands::clearColourBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLClearColourBufferCommand();
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	RenderCommands* RenderCommands::clearDepthBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLClearDepthBufferCommand();
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	RenderCommands* RenderCommands::setClearColourCommand(float r, float g, float b, float a)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetClearColourCommand(r, g, b, a);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	RenderCommands* RenderCommands::setDepthTestCommand(bool enabled)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetDepthTestCommand(enabled);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	RenderCommands* RenderCommands::setBlendCommand(bool enabled)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetBlendCommand(enabled);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	RenderCommands* RenderCommands::setTransparencyBlend()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetTransparencyBlendCommand();
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	RenderCommands* RenderCommands::setFaceCullCommand(bool enabled)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLSetFaceCullCommand(enabled);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	RenderCommands* RenderCommands::clearScreenCommand(float r, float g, float b, float a)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLClearScreenCommand(r, g, b, a);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}
	}

	/*std::function<void(void)> RenderCommandFactory::getClearDepthBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Direct3D:
			return std::function<void(void)>();
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return []() { glClear(GL_DEPTH_BUFFER_BIT); };
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}

	std::function<void(void)> RenderCommandFactory::getClearColourBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Direct3D:
			return std::function<void(void)>();
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return []() { glClear(GL_COLOR_BUFFER_BIT); };
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}

	std::function<void(void)> RenderCommandFactory::getClearColourAndDepthBufferCommand()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Direct3D:
			return std::function<void(void)>();
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return []() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}

	std::function<void(void)> RenderCommandFactory::getSetClearColourCommand(float r, float g, float b, float a)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Direct3D:
			return std::function<void(void)>();
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			return [r, g, b, a]() { glClearColor(r, g, b, a); };
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}

	std::function<void(void)> RenderCommandFactory::getSetDepthTest(bool enabled)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::Direct3D:
			return std::function<void(void)>();
		case RenderAPI::API::None:
			return std::function<void(void)>();
		case RenderAPI::API::OpenGL:
			if (enabled) return []() { glEnable(GL_DEPTH_TEST); };
			else return []() { glDisable(GL_DEPTH_TEST); };
		case RenderAPI::API::Vulkan:
			return std::function<void(void)>();
		default:
			return std::function<void(void)>();
		}
	}*/
}