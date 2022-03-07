/** \file renderer3D.cpp */

#include "engine_pch.h"
#include <glad/glad.h>
#include "rendering/renderer3D.h"

namespace Engine
{
	std::shared_ptr<Renderer3D::InternalData> Renderer3D::s_data = nullptr;

	VertexBufferLayout TPVertexNormalised::s_layout = { {ShaderDataType::Float3, {ShaderDataType::Short3, true}, {ShaderDataType::Short2, true} }, 24 }; //!< Layout for data in TPVertexNormalised

	void Renderer3D::init()
	{
		s_data.reset(new InternalData);

		unsigned char whitePx[4] = { 255, 255, 255, 255 };
		s_data->defaultTexture.reset(Texture::create(1, 1, 4, whitePx));

		s_data->defaultTint = { 1.f, 1.f, 1.f, 1.f };
	}

	void Renderer3D::begin(const SceneWideUniforms& sceneWideUniforms)
	{
		s_data->sceneWideUniforms = sceneWideUniforms;
	}

	void Renderer3D::submit(const std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material>& material, const glm::mat4& model)
	{
		// Bind shader
		glUseProgram(material->getShader()->getID());

		// Apply material uniforms (per draw uniform)
		material->getShader()->uploadMat4("u_model", model);

		std::shared_ptr<Texture> texture;
		if (material->isFlagSet(Material::flag_texture)) texture = material->getTexture();
		else texture = s_data->defaultTexture;

		uint32_t texUnit;
		if (RendererCommon::texUnitMan.getUnit(texture->getID(), texUnit))
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, texture->getID());
		}

		material->getShader()->uploadInt("u_texData", texUnit);

		if (material->isFlagSet(Material::flag_tint)) material->getShader()->uploadFloat4("u_tint", material->getTint());
		else material->getShader()->uploadFloat4("u_tint", s_data->defaultTint);

		// Bind the geometry (VAO and IBO)
		glBindVertexArray(geometry->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->getIndexBuffer()->getID());

		// Submit the draw call
		glDrawElements(GL_TRIANGLES, geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer3D::end()
	{
		s_data->sceneWideUniforms.clear();
	}

	std::array<int16_t, 3> TPVertexNormalised::normalise(const glm::vec3& normal)
	{
		std::array<int16_t, 3> result;

		if (normal.x == 1.0) result.at(0) = INT16_MAX;
		else if (normal.x == -1.0) result.at(0) = INT16_MIN;
		else result.at(0) = static_cast<int16_t>(normal.x * static_cast<float>(INT16_MAX));

		if (normal.y == 1.0) result.at(1) = INT16_MAX;
		else if (normal.y == -1.0) result.at(1) = INT16_MIN;
		else result.at(1) = static_cast<int16_t>(normal.y * static_cast<float>(INT16_MAX));

		if (normal.z == 1.0) result.at(2) = INT16_MAX;
		else if (normal.z == -1.0) result.at(2) = INT16_MIN;
		else result.at(2) = static_cast<int16_t>(normal.z * static_cast<float>(INT16_MAX));

		return result;
	};

	std::array<int16_t, 2> TPVertexNormalised::normalise(const glm::vec2& uv)
	{
		std::array<int16_t, 2> result;

		if (uv.x == 1.0) result.at(0) = INT16_MAX;
		else if (uv.x == -1.0) result.at(0) = INT16_MIN;
		else result.at(0) = static_cast<int16_t>(uv.x * static_cast<float>(INT16_MAX));

		if (uv.y == 1.0) result.at(1) = INT16_MAX;
		else if (uv.y == -1.0) result.at(1) = INT16_MIN;
		else result.at(1) = static_cast<int16_t>(uv.y * static_cast<float>(INT16_MAX));

		return result;
	};
}