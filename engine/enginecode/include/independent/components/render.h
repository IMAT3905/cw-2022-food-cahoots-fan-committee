/* \file render.h */
#pragma once

#include "rendering/renderer3D.h"
#include "systems/log.h"

namespace Engine
{
	class RenderComponent
	{
	public: 
		RenderComponent(const Geometry pGeometry, std::shared_ptr<Material> pMaterial) { geometry = pGeometry; material = pMaterial; }
		RenderComponent(const Geometry pGeometry, const std::shared_ptr<Shader>& shader)
		{
			geometry = pGeometry;
			material.reset(new Material(shader));
		}
		RenderComponent(const Geometry pGeometry, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture, const glm::vec4& tint)
		{
			geometry = pGeometry;
			material.reset(new Material(shader, texture, tint));
		}
		RenderComponent(const Geometry pGeometry, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture)
		{
			geometry = pGeometry;
			material.reset(new Material(shader, texture));
		}
		RenderComponent(const Geometry pGeometry, const std::shared_ptr<Shader>& shader, const glm::vec4& tint)
		{
			geometry = pGeometry;
			material.reset(new Material(shader, tint));
		}

		Geometry geometry;
		std::shared_ptr<Material> material;
	};
}