/* \file rendererCommon.cpp */
#include "engine_pch.h"
#include "rendering/rendererCommon.h"


namespace Engine
{
	std::shared_ptr<Texture> RendererCommon::defaultTexture = nullptr;
	std::vector<int32_t> RendererCommon::textureUnits;
	SubTexture RendererCommon::defaultSubTexture;
	TextureUnitManager RendererCommon::texUnitMan = TextureUnitManager(32);

	uint32_t RendererCommon::pack(const glm::vec4& tint)
	{
		uint32_t result = 0;

		uint32_t r = (static_cast<uint32_t>(tint.r * 255.f)) << 0;
		uint32_t g = (static_cast<uint32_t>(tint.g * 255.f)) << 8;
		uint32_t b = (static_cast<uint32_t>(tint.b * 255.f)) << 16;
		uint32_t a = (static_cast<uint32_t>(tint.a * 255.f)) << 24;

		result = (r | g | b | a);
		return result;
	}
}


