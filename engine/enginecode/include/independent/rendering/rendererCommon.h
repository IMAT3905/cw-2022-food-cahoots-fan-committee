/** \file rendererCommon.h */
#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>

#include "rendering/shaderDataType.h"
#include "rendering/vertexArray.h"
#include "rendering/texture.h"
#include "rendering/subTexture.h"
#include "rendering/shader.h"
#include "rendering/RenderCommands.h"
#include "rendering/uniformBuffer.h"
#include "rendering/textureUnitManager.h"
#include "rendering/textureAtlas.h"

namespace Engine
{
	using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void*>>;

	/** \class RendererCommon
	** \brief A class to hold data that is used in all rendering
	*/
	class RendererCommon
	{
	public:
		static void actionCommand(RenderCommands* command) { command->action(); delete command; }; //!< A function to run the command specified \param command is the command that will be ran
		static TextureUnitManager texUnitMan; //!< The texture unit manager
	};
}