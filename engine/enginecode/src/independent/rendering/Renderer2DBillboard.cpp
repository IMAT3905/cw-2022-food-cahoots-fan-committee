#include "engine_pch.h"
#include "systems/log.h"
#include <glad/glad.h>
#include "rendering/Renderer2DBillboard.h"
#include "core/application.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numeric>

namespace Engine
{
	VertexBufferLayout BillboardVertex::layout = VertexBufferLayout({ ShaderDataType::Float2, ShaderDataType::Float3, ShaderDataType::Float2, ShaderDataType::FlatInt, {ShaderDataType::Byte4, true} });

	std::shared_ptr<Renderer2DBillboard::InternalData> Renderer2DBillboard::s_data = nullptr;

	void Renderer2DBillboard::init(uint32_t batchSize)
	{
		s_data.reset(new InternalData);
		s_data->batchSize = batchSize * 4;

		std::shared_ptr<Texture> CommonTexture = RendererCommon::defaultTexture;

		if (!RendererCommon::defaultTexture)
		{
			unsigned char whitePx[4] = { 255, 255, 255, 255 };
			RendererCommon::defaultTexture.reset(Texture::create(1, 1, 4, whitePx));
			RendererCommon::defaultSubTexture = SubTexture(RendererCommon::defaultTexture, glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f));
		}

		s_data->shader.reset(Shader::create("./assets/shaders/quad6.glsl"));

		s_data->UBO.reset(UniformBuffer::create(UniformBufferLayout({
			{ "u_view", ShaderDataType::Mat4 },
			{ "u_projection", ShaderDataType::Mat4 }
			})));

		s_data->quad[0] = { -0.5f, -0.5f, 1.f, 1.f };
		s_data->quad[1] = { -0.5f,  0.5f, 1.f, 1.f };
		s_data->quad[2] = { 0.5f,  0.5f, 1.f, 1.f };
		s_data->quad[3] = { 0.5f, -0.5f, 1.f, 1.f };

		s_data->vertices.resize(s_data->batchSize);

		std::vector<uint32_t> indices(s_data->batchSize);
		std::iota(indices.begin(), indices.end(), 0);

		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<IndexBuffer> IBO;

		s_data->VAO.reset(VertexArray::create());
		VBO.reset(VertexBuffer::create(s_data->vertices.data(), sizeof(BillboardVertex) * s_data->vertices.size(), BillboardVertex::layout));
		IBO.reset(IndexBuffer::create(indices.data(), indices.size()));
		s_data->VAO->addVertexBuffer(VBO);
		s_data->VAO->setIndexBuffer(IBO);

		s_data->UBO->attachShaderBlock(s_data->shader, "b_camera");
	}

	void Renderer2DBillboard::submit(const BillboardQuad& quad, const glm::vec4& tint, const SubTexture& texture)
	{
		if (s_data->drawCount + 4 > s_data->batchSize) flush();

		uint32_t packedTint = RendererCommon::pack(tint);

		uint32_t textureUnit;
		if (RendererCommon::texUnitMan.full()) flush();
		if (RendererCommon::texUnitMan.getUnit(texture.getBaseTexture()->getID(), textureUnit))
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, texture.getBaseTexture()->getID());
		}

		uint32_t startIdx = s_data->drawCount;

		for (int i = 0; i < 4; i++)
		{
			s_data->vertices[i + startIdx].position = glm::vec2(s_data->quad[i].x * quad.m_size.x, s_data->quad[i].y * quad.m_size.y);
			s_data->vertices[i + startIdx].centre = quad.m_centre;
			s_data->vertices[i + startIdx].tint = packedTint;
			s_data->vertices[i + startIdx].texUnit = textureUnit;
		}
		s_data->vertices[startIdx + 0].uvCoords = texture.getUVStart();
		s_data->vertices[startIdx + 1].uvCoords = { texture.getUVStart().x, texture.getUVEnd().y };
		s_data->vertices[startIdx + 2].uvCoords = texture.getUVEnd();
		s_data->vertices[startIdx + 3].uvCoords = { texture.getUVEnd().x, texture.getUVStart().y };

		s_data->drawCount += 4;
	}

	void Renderer2DBillboard::begin(const SceneWideUniforms& swu)
	{
		//reset draw count
		s_data->drawCount = 0;

		//bind the shader
		glUseProgram(s_data->shader->getID());

		//s_data->shader->uploadIntArray("u_texData", RendererCommon::textureUnits.data(), 32);

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->UBO->getID());
		s_data->UBO->uploadData("u_projection", swu.at("u_projection").second);
		s_data->UBO->uploadData("u_view", swu.at("u_view").second);
	}

	void Renderer2DBillboard::end()
	{
		if (s_data->drawCount > 0) flush();
	}

	void Renderer2DBillboard::flush()
	{
		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->vertices.data(), sizeof(BillboardVertex) * s_data->drawCount, 0);

		glBindVertexArray(s_data->VAO->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());

		glDrawElements(GL_QUADS, s_data->drawCount, GL_UNSIGNED_INT, nullptr);
		s_data->drawCount = 0;
	}
}