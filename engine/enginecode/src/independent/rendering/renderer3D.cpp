/** \file renderer3D.cpp */

#include "engine_pch.h"
#include <glad/glad.h>
#include "rendering/renderer3D.h"

namespace Engine
{
	std::shared_ptr<Renderer3D::InternalData> Renderer3D::s_data = nullptr;

	VertexBufferLayout Renderer3DVertex::layout = VertexBufferLayout({ShaderDataType::Float3, ShaderDataType::Float3, ShaderDataType::Float2 }); //!< Layout for data in TPVertexNormalised

	void Renderer3D::init(uint32_t vertexCapacity, uint32_t indexCapacity, uint32_t batchSize)
	{
		if (!RendererCommon::defaultTexture)
		{
			unsigned char whitePx[4] = { 255, 255, 255, 255 };
			RendererCommon::defaultTexture.reset(Texture::create(1, 1, 4, whitePx));
			RendererCommon::defaultSubTexture = SubTexture(RendererCommon::defaultTexture, glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f));
		}

		s_data.reset(new InternalData);

		s_data->batchCapacity = batchSize;
		s_data->vertexCapacity = vertexCapacity;
		s_data->indexCapacity = indexCapacity;

		s_data->batchQueue.reserve(batchSize);
		s_data->modelInstanceData.reserve(batchSize);
		s_data->tintInstanceData.reserve(batchSize);
		s_data->texInstanceData.reserve(batchSize);
		s_data->batchCommands.reserve(batchSize);

		s_data->VAO.reset(VertexArray::create());

		std::shared_ptr<VertexBuffer> VBOverts;
		std::shared_ptr<IndexBuffer> IBO;

		VBOverts.reset(VertexBuffer::create(nullptr, sizeof(Renderer3DVertex) * vertexCapacity, Renderer3DVertex::layout));
		IBO.reset(IndexBuffer::create(nullptr, indexCapacity));

		s_data->VAO->addVertexBuffer(VBOverts);
		s_data->VAO->setIndexBuffer(IBO);

		VertexBufferLayout modelLayout = { { ShaderDataType::Mat4, 1, false } };
		std::shared_ptr<VertexBuffer> VBOmodels;
		VBOmodels.reset(VertexBuffer::create(nullptr, batchSize * sizeof(glm::mat4), modelLayout));
		s_data->VAO->addVertexBuffer(VBOmodels);

		VertexBufferLayout texLayout = { { ShaderDataType::FlatInt, 1, false } };
		std::shared_ptr<VertexBuffer> VBOTexUnits;
		VBOTexUnits.reset(VertexBuffer::create(nullptr, batchSize * sizeof(uint32_t), texLayout));
		s_data->VAO->addVertexBuffer(VBOTexUnits);

		VertexBufferLayout tintLayout = { { ShaderDataType::Byte4, 1, true } };
		std::shared_ptr<VertexBuffer> VBOTints;
		VBOTints.reset(VertexBuffer::create(nullptr, batchSize * sizeof(uint32_t), tintLayout));
		s_data->VAO->addVertexBuffer(VBOTints);

		s_data->commands.reset(IndirectBuffer::create(nullptr, batchSize));

		s_data->cameraUBO.reset(UniformBuffer::create(UniformBufferLayout({
			{"u_projection", ShaderDataType::Mat4},
			{"u_view", ShaderDataType::Mat4}
			})));

		s_data->lightsUBO.reset(UniformBuffer::create(UniformBufferLayout({
			{"u_lightPos", ShaderDataType::Float3},
			{"u_viewPos", ShaderDataType::Float3},
			{"u_lightColour", ShaderDataType::Float3}
			})));
	}

	void Renderer3D::begin(const SceneWideUniforms& sceneWideUniforms)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, s_data->cameraUBO->getID());
		s_data->cameraUBO->uploadData("u_projection", sceneWideUniforms.at("u_projection").second);
		s_data->cameraUBO->uploadData("u_view", sceneWideUniforms.at("u_view").second);

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->lightsUBO->getID());
		s_data->lightsUBO->uploadData("u_lightPos", sceneWideUniforms.at("u_lightPos").second);
		s_data->lightsUBO->uploadData("u_viewPos", sceneWideUniforms.at("u_viewPos").second);
		s_data->lightsUBO->uploadData("u_lightColour", sceneWideUniforms.at("u_lightColour").second);	
	}

	void Renderer3D::submit(const Geometry& geometry, const std::shared_ptr<Material>& material, const glm::mat4& model)
	{
		if (material->isFlagSet(Material::flag_batched))
		{
			if (s_data->batchQueue.size() == s_data->batchCapacity) flushBatch();

			s_data->batchQueue.push_back({ geometry, material, model });
		}
		else
		{
			// Bind shader
			glUseProgram(material->getShader()->getID());

			if (material->isFlagSet(Material::flag_batched)) material->getShader()->uploadIntArray("u_texData", RendererCommon::textureUnits.data(), 32);

			uint32_t texUnit;
			if (RendererCommon::texUnitMan.getUnit(material->getTexture()->getID(), texUnit))
			{
				glActiveTexture(GL_TEXTURE0 + texUnit);
				glBindTexture(GL_TEXTURE_2D, material->getTexture()->getID());
			}

			// Apply material uniforms (per draw uniform)
			material->getShader()->uploadMat4("a_model", model);
			material->getShader()->uploadInt("a_texData", texUnit);
			material->getShader()->uploadFloat4("a_tint", material->getTint());

			// Bind the geometry (VAO and IBO)
			glBindVertexArray(s_data->VAO->getID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());

			// Submit the draw call
			glDrawElementsBaseVertex(GL_TRIANGLES, geometry.indexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * geometry.firstIndex), geometry.firstVertex);
		}
	}

	void Renderer3D::end()
	{
		flush();
	}

	void Renderer3D::flush()
	{
		if (s_data->batchQueue.size() > 0) flushBatch();
	}

	void Renderer3D::initShader(std::shared_ptr<Shader> shader)
	{
		s_data->cameraUBO->attachShaderBlock(shader, "b_camera");
		s_data->lightsUBO->attachShaderBlock(shader, "b_lights");
	}

	bool Renderer3D::addGeometry(std::vector<Renderer3DVertex> vertices, std::vector<uint32_t> indices, Geometry& geo)
	{
		uint32_t vertexCount = vertices.size();
		uint32_t indexCount = indices.size();

		if (vertexCount + s_data->nextVertex > s_data->vertexCapacity) return false;
		if (indexCount + s_data->nextIndex > s_data->indexCapacity) return false;

		auto VBO = s_data->VAO->getVertexBuffers().at(0);
		auto IBO = s_data->VAO->getIndexBuffer();

		VBO->edit(vertices.data(), vertexCount * sizeof(Renderer3DVertex), s_data->nextVertex * sizeof(Renderer3DVertex));
		IBO->edit(indices.data(), indexCount, s_data->nextIndex);

		geo.id = s_data->batchCommands.size();
		geo.firstIndex = s_data->nextIndex;
		geo.firstVertex = s_data->nextVertex;
		geo.indexCount = indexCount;
		geo.vertexCount = vertexCount;

		s_data->nextVertex += vertexCount;
		s_data->nextIndex += indexCount;

		s_data->batchCommands.push_back({ 0,0,0,0,0 });
		return true;
	}

	void Renderer3D::flushBatch()
	{
		// Sort batch queue, by shader then geometry id
		std::sort(s_data->batchQueue.begin(), s_data->batchQueue.end(),
			[](BatchQueueEntry& a, BatchQueueEntry& b) {
				if (a.material->getShader() == b.material->getShader())
				{
					return a.geometry.id < b.geometry.id;
				}
				return a.material->getShader() < b.material->getShader();
			}
		);

		uint32_t runningInstanceCount = 0;
		uint32_t texUnit;
		
		for (auto& bqe : s_data->batchQueue)
		{
			auto& index = bqe.geometry.id;

			if (s_data->batchCommands.at(index).drawCount == 0)
			{
				s_data->batchCommands.at(index).firstVertex = bqe.geometry.firstVertex;
				s_data->batchCommands.at(index).firstIndex = bqe.geometry.firstIndex;
				s_data->batchCommands.at(index).drawCount = bqe.geometry.indexCount;
				s_data->batchCommands.at(index).firstInstance = runningInstanceCount;
			}

			s_data->batchCommands.at(index).instanceCount++;
			runningInstanceCount++;

			s_data->modelInstanceData.push_back(bqe.model);
			s_data->tintInstanceData.push_back(RendererCommon::pack(bqe.material->getTint()));

			if (RendererCommon::texUnitMan.full())
			{
				flushBatchCommands(bqe.material->getShader(), runningInstanceCount);
				runningInstanceCount = 0;
			}
			if (RendererCommon::texUnitMan.getUnit(bqe.material->getTexture()->getID(), texUnit))
			{
				glActiveTexture(GL_TEXTURE0 + texUnit);
				glBindTexture(GL_TEXTURE_2D, bqe.material->getTexture()->getID());
			}

			s_data->texInstanceData.push_back(texUnit);
		}

		if (runningInstanceCount > 0)
		{
			flushBatchCommands(s_data->batchQueue.back().material->getShader(), runningInstanceCount);
			runningInstanceCount = 0;
		}
		s_data->batchQueue.clear();
		s_data->modelInstanceData.clear();
		s_data->texInstanceData.clear();
		s_data->tintInstanceData.clear();

		for (auto& command : s_data->batchCommands) command.instanceCount = 0;
	}

	void Renderer3D::flushBatchCommands(std::shared_ptr<Shader>& shader, uint32_t instanceCount)
	{
		glUseProgram(shader->getID());
		shader->uploadIntArray("u_texData", RendererCommon::textureUnits.data(), 32);

		auto VBOmodels = s_data->VAO->getVertexBuffers().at(1);
		auto VBOtexUnits = s_data->VAO->getVertexBuffers().at(2);
		auto VBOtints = s_data->VAO->getVertexBuffers().at(3);

		VBOmodels->edit(s_data->modelInstanceData.data(), sizeof(glm::mat4) * instanceCount, 0);
		VBOtexUnits->edit(s_data->texInstanceData.data(), sizeof(uint32_t) * instanceCount, 0);
		VBOtints->edit(s_data->tintInstanceData.data(), sizeof(uint32_t) * instanceCount, 0);

		glBindVertexArray(s_data->VAO->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());

		s_data->commands->edit(s_data->batchCommands.data(), s_data->batchCommands.size(), 0);

		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0, s_data->batchCommands.size(), 0);
	}
}