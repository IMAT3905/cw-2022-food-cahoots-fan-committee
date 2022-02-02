/* \file render.h */
#pragma once

#include "rendering/renderer3D.h"
#include "systems/log.h"

namespace Engine
{
	class RenderComponent
	{
	public: 
		RenderComponent(const std::shared_ptr<VertexArray>& pGeometry, std::shared_ptr<Material> pMaterial) { geometry = pGeometry; material = pMaterial; }
		RenderComponent(const std::shared_ptr<VertexArray>& pGeometry, const std::shared_ptr<Shader>& shader)
		{
			geometry = pGeometry;
			material.reset(new Material(shader));
		}
		RenderComponent(const std::shared_ptr<VertexArray>& pGeometry, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture, const glm::vec4& tint)
		{
			geometry = pGeometry;
			material.reset(new Material(shader, texture, tint));
		}
		RenderComponent(const std::shared_ptr<VertexArray>& pGeometry, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture)
		{
			geometry = pGeometry;
			material.reset(new Material(shader, texture));
		}
		RenderComponent(const std::shared_ptr<VertexArray>& pGeometry, const std::shared_ptr<Shader>& shader, const glm::vec4& tint)
		{
			geometry = pGeometry;
			material.reset(new Material(shader, tint));
		}

		std::shared_ptr<VertexArray> geometry;
		std::shared_ptr<Material> material;
	};
}