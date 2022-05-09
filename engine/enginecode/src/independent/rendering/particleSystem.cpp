#include "engine_pch.h"
#include "systems/log.h"
#include <glad/glad.h>
#include "rendering/particleSystem.h"
#include "core/application.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <numeric>

namespace Engine
{
	/*VertexBufferLayout ParticleVertex::layout = VertexBufferLayout({ShaderDataType::Float2, ShaderDataType::Float3, ShaderDataType::Float2, ShaderDataType::FlatInt, ShaderDataType::Float4});

	std::shared_ptr<ParticleSystem::InternalData> ParticleSystem::s_data = nullptr;


	void ParticleSystem::init(uint32_t particleCapacity)
	{
		s_data.reset(new InternalData);

		s_data->particleBatchSize = particleCapacity * 4;
		s_data->particleCapacity = particleCapacity;
		s_data->textureUnit = 31;
		s_data->nbParticleDrawCount = 0;
		s_data->mbParticleDrawCount = 0;
		s_data->abParticleDrawCount = 0;

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

		s_data->nonBlendedVertices.resize(s_data->particleBatchSize);
		s_data->mixBlendedVertices.resize(s_data->particleBatchSize);
		s_data->additiveBlendedVertices.resize(s_data->particleBatchSize);
		s_data->nonBlendedParticles.reserve(s_data->particleCapacity);
		s_data->mixBlendedParticles.reserve(s_data->particleCapacity);
		s_data->additiveBlendedParticles.reserve(s_data->particleCapacity);

		std::vector<uint32_t> indices(s_data->particleBatchSize);
		std::iota(indices.begin(), indices.end(), 0);

		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<IndexBuffer> IBO;

		s_data->VAO.reset(VertexArray::create());
		VBO.reset(VertexBuffer::create(s_data->nonBlendedVertices.data(), sizeof(ParticleVertex) * s_data->nonBlendedVertices.size(), ParticleVertex::layout));
		IBO.reset(IndexBuffer::create(indices.data(), indices.size()));
		s_data->VAO->addVertexBuffer(VBO);
		s_data->VAO->setIndexBuffer(IBO);

		s_data->UBO->attachShaderBlock(s_data->shader, "b_camera");

		s_data->atlas.reset(new TextureAtlas({ 4096, 4096 }, 4, 100));
		s_data->particleTextures.reserve(100);
	}

	void ParticleSystem::addParticle(Particle& p)
	{
		switch (p.blendMode)
		{
		case BlendModes::None:
			if (s_data->nbParticleDrawCount < s_data->particleBatchSize) s_data->nonBlendedParticles.push_back(p);
			break;
		case BlendModes::Mix:
			if (s_data->mbParticleDrawCount < s_data->particleBatchSize) s_data->mixBlendedParticles.push_back(p);
			break;
		case BlendModes::Additive:
			if (s_data->abParticleDrawCount < s_data->particleBatchSize) s_data->additiveBlendedParticles.push_back(p);
			break;
		}
	}

	void ParticleSystem::onUpdate(float timestep, glm::vec3& camera)
	{
		s_data->nbParticleDrawCount = 0;
		for (int i = 0; i < s_data->nonBlendedParticles.size(); i++)
		{
			s_data->nonBlendedParticles[i].onUpdate(timestep);

			if (s_data->nonBlendedParticles[i].destroyMe())
			{
				if (i != s_data->nonBlendedParticles.size() - 1)
				{
					s_data->nonBlendedParticles[i] = s_data->nonBlendedParticles[s_data->nonBlendedParticles.size() - 1];
				}
				s_data->nonBlendedParticles.pop_back();
				i--;
			}

			if (1 < i && i < s_data->particleCapacity) ParticleSystem::addParticleRenderData(s_data->nonBlendedParticles[i]);
			else if (i >= (int)s_data->particleCapacity)
			{
				Log::error("Non-blended particles full");
			}
		}

		std::sort(s_data->mixBlendedParticles.begin(), s_data->mixBlendedParticles.begin() + s_data->mbParticleDrawCount,
			[camera](const Particle& p1, const Particle& p2) -> bool
			{
				return glm::length2(camera - p1.deviceProps.linearPosition) < glm::length2(camera - p2.deviceProps.linearPosition);
			});

		s_data->mbParticleDrawCount = 0;

		for (int i = 0; i < s_data->mixBlendedParticles.size(); i++)
		{
			s_data->mixBlendedParticles[i].onUpdate(timestep);

			if (s_data->mixBlendedParticles[i].destroyMe())
			{
				if (i != s_data->mixBlendedParticles.size() - 1)
				{
					s_data->mixBlendedParticles[i] = s_data->mixBlendedParticles[s_data->mixBlendedParticles.size() - 1];
				}
				s_data->mixBlendedParticles.pop_back();
				i--;
			}

			if (1 < i && i < s_data->particleCapacity) ParticleSystem::addParticleRenderData(s_data->mixBlendedParticles[i]);
			else if (i >= (int)s_data->particleCapacity)
			{
				Log::error("Mix-blended particles full");
			}
		}

		std::sort(s_data->additiveBlendedParticles.begin(), s_data->additiveBlendedParticles.begin() + s_data->abParticleDrawCount,
			[camera](const Particle& p1, const Particle& p2) -> bool
			{
				return glm::length2(camera - p1.deviceProps.linearPosition) < glm::length2(camera - p2.deviceProps.linearPosition);
			});

		s_data->abParticleDrawCount = 0;

		for (int i = 0; i < s_data->additiveBlendedParticles.size(); i++)
		{
			s_data->additiveBlendedParticles[i].onUpdate(timestep);

			if (s_data->additiveBlendedParticles[i].destroyMe())
			{
				if (i != s_data->additiveBlendedParticles.size() - 1)
				{
					s_data->additiveBlendedParticles[i] = s_data->additiveBlendedParticles[s_data->additiveBlendedParticles.size() - 1];
				}
				s_data->additiveBlendedParticles.pop_back();
				i--;
			}

			if (1 < i && i < s_data->particleCapacity) ParticleSystem::addParticleRenderData(s_data->additiveBlendedParticles[i]);
			else if (i >= (int)s_data->particleCapacity)
			{
				Log::error("Additive-blended particles full");
			}
		}
	}

	void ParticleSystem::onRender(const SceneWideUniforms& swu)
	{
		glUseProgram(s_data->shader->getID());

		s_data->shader->uploadIntArray("u_texData", RendererCommon::textureUnits.data(), 32);

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->UBO->getID());
		s_data->UBO->uploadData("u_projection", swu.at("u_projection").second);
		s_data->UBO->uploadData("u_view", swu.at("u_view").second);

		glActiveTexture(GL_TEXTURE0 + s_data->textureUnit);
		glBindTexture(GL_TEXTURE_2D, s_data->atlas->getBaseTexture()->getID());

		glBindVertexArray(s_data->VAO->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());

		//draw non blended particles
		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->nonBlendedVertices.data(), sizeof(ParticleVertex) * s_data->nbParticleDrawCount, 0);
		glDrawElements(GL_QUADS, s_data->nbParticleDrawCount, GL_UNSIGNED_INT, nullptr);
		s_data->nbParticleDrawCount = 0;

		//draw mix blended particles
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->mixBlendedVertices.data(), sizeof(ParticleVertex) * s_data->mbParticleDrawCount, 0);
		glDrawElements(GL_QUADS, s_data->mbParticleDrawCount, GL_UNSIGNED_INT, nullptr);
		s_data->mbParticleDrawCount = 0;

		//draw additive blended particles
		glBlendFunc(GL_ONE, GL_ONE);
		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->additiveBlendedVertices.data(), sizeof(ParticleVertex) * s_data->abParticleDrawCount, 0);
		glDrawElements(GL_QUADS, s_data->abParticleDrawCount, GL_UNSIGNED_INT, nullptr);
		s_data->abParticleDrawCount = 0;
		glDisable(GL_BLEND);
	}

	bool ParticleSystem::addTexture(const char* filepath, uint32_t& index)
	{
		SubTexture t;
		if (s_data->atlas->add(filepath, t))
		{
			s_data->particleTextures.push_back(t);
			index = s_data->particleTextures.size() - 1;
			return true;
		}
		index = 0xFFFFFFFF;
		return false;
	}

	bool ParticleSystem::getUVs(uint32_t index, glm::vec2& UV_start, glm::vec2& UV_end)
	{
		if (index < s_data->particleTextures.size())
		{
			UV_start = s_data->particleTextures[index].getUVStart();
			UV_end = s_data->particleTextures[index].getUVEnd();
			return true;
		}
		return false;
	}

	void ParticleSystem::addParticleRenderData(Particle& p)
	{
		auto packedTint = RendererCommon::pack(p.deviceProps.currentColour);
		glm::mat4 model = glm::scale(glm::rotate(glm::mat4(1.f), p.deviceProps.angularPosition, { 0.f, 0.f, 1.f }), glm::vec3(p.deviceProps.currentSize, 1.0f));

		std::vector<ParticleVertex>* pRenderVerts = nullptr;

		uint32_t startIdx = -1;
		switch (p.blendMode)
		{
		case BlendModes::None:
			pRenderVerts = &s_data->nonBlendedVertices;
			startIdx = s_data->nbParticleDrawCount;
			s_data->nbParticleDrawCount += 4;
			break;
		case BlendModes::Mix:
			pRenderVerts = &s_data->mixBlendedVertices;
			startIdx = s_data->mbParticleDrawCount;
			s_data->mbParticleDrawCount += 4;
			break;
		case BlendModes::Additive:
			pRenderVerts = &s_data->additiveBlendedVertices;
			startIdx = s_data->abParticleDrawCount;
			s_data->abParticleDrawCount += 4;
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			(*pRenderVerts)[i + startIdx].position = model * s_data->quad[i];
			(*pRenderVerts)[i + startIdx].centre = p.deviceProps.linearPosition;
			(*pRenderVerts)[i + startIdx].tint = packedTint;
			(*pRenderVerts)[i + startIdx].texUnit = s_data->textureUnit;
		}
		(*pRenderVerts)[startIdx + 0].uvCoords = p.deviceProps.current_UVStart;
		(*pRenderVerts)[startIdx + 1].uvCoords = { p.deviceProps.current_UVStart.x, p.deviceProps.current_UVEnd.y };
		(*pRenderVerts)[startIdx + 2].uvCoords = p.deviceProps.current_UVEnd;
		(*pRenderVerts)[startIdx + 3].uvCoords = { p.deviceProps.current_UVEnd.x, p.deviceProps.current_UVStart.y };
	}



	Particle::Particle(ParticleHostProprties& Hprops, particleDeviceProperties& Dprops, BlendModes p_blendMode) :
		hostProps(Hprops), deviceProps(Dprops), blendMode(p_blendMode)
	{
		float r, g, b, a, scale;
		glm::vec4 delta4;
		glm::vec3 delta3;

		switch (hostProps.colourRandomType)
		{
		case RandomTypes::None:
			deviceProps.currentColour = hostProps.startColour;
			break;
		case RandomTypes::Normal:
			r = Randomiser::normalFloat(0, hostProps.colourRandomisation.x);
			g = Randomiser::normalFloat(0, hostProps.colourRandomisation.y);
			b = Randomiser::normalFloat(0, hostProps.colourRandomisation.z);
			a = hostProps.startColour.a;
			delta4 = glm::vec4(r, g, b, 0.f);
			hostProps.startColour += delta4;
			hostProps.endColour -= delta4;
			deviceProps.currentColour = hostProps.startColour;
			break;
		case RandomTypes::Uniform:
			r = Randomiser::uniformFloatBetween(-hostProps.colourRandomisation.r, hostProps.colourRandomisation.r);
			g = Randomiser::uniformFloatBetween(-hostProps.colourRandomisation.g, hostProps.colourRandomisation.g);
			b = Randomiser::uniformFloatBetween(-hostProps.colourRandomisation.b, hostProps.colourRandomisation.b);
			a = hostProps.startColour.a;

			delta4 = glm::vec4(r, g, b, 0.f);
			hostProps.startColour += delta4;
			hostProps.endColour - +delta4;
			deviceProps.currentColour = hostProps.startColour;

			break;
		}

		switch (hostProps.scaleRandomType)
		{
		case RandomTypes::None:
			deviceProps.currentSize = hostProps.startSize;
			break;
		case RandomTypes::Normal:
			scale = Randomiser::normalFloat(-hostProps.scaleRandomisation, hostProps.scaleRandomisation);
			hostProps.startSize -= scale;
			hostProps.endSize += scale;
			deviceProps.currentSize = hostProps.startSize;
			break;
		case RandomTypes::Uniform:
			scale = Randomiser::normalFloat(1.0, hostProps.scaleRandomisation);
			hostProps.startSize -= scale;
			hostProps.endSize += scale;
			deviceProps.currentSize = hostProps.startSize;
			break;
		}

		switch (hostProps.positionRandomType)
		{
		case RandomTypes::Normal:
			delta3.x = Randomiser::normalFloat(deviceProps.linearPosition.x, hostProps.positionRandomisation.x);
			delta3.y = Randomiser::normalFloat(deviceProps.linearPosition.y, hostProps.positionRandomisation.y);
			delta3.z = Randomiser::normalFloat(deviceProps.linearPosition.z, hostProps.positionRandomisation.z);
			deviceProps.linearPosition = delta3;
			break;
		case RandomTypes::Uniform:
			delta3.x = Randomiser::uniformFloatBetween(-hostProps.positionRandomisation.x, hostProps.positionRandomisation.x);
			delta3.y = Randomiser::uniformFloatBetween(-hostProps.positionRandomisation.y, hostProps.positionRandomisation.y);
			delta3.z = Randomiser::uniformFloatBetween(-hostProps.positionRandomisation.z, hostProps.positionRandomisation.z);
			deviceProps.linearPosition = delta3;
			break;
		}

		switch (hostProps.velocityRandomType)
		{
		case RandomTypes::Normal:
			delta3.x = Randomiser::normalFloat(hostProps.linearVelocity.x, hostProps.velocityRandomisation.x);
			delta3.y = Randomiser::normalFloat(hostProps.linearVelocity.y, hostProps.velocityRandomisation.y);
			delta3.z = Randomiser::normalFloat(hostProps.linearVelocity.z, hostProps.velocityRandomisation.z);
			hostProps.linearVelocity = delta3;
			break;
		case RandomTypes::Uniform:
			delta3.x = Randomiser::uniformFloatBetween(-hostProps.velocityRandomisation.x, hostProps.velocityRandomisation.x);
			delta3.y = Randomiser::uniformFloatBetween(-hostProps.velocityRandomisation.y, hostProps.velocityRandomisation.y);
			delta3.z = Randomiser::uniformFloatBetween(-hostProps.velocityRandomisation.z, hostProps.velocityRandomisation.z);
			hostProps.linearVelocity = delta3;
			break;
		}
	}

	void Particle::onUpdate(float timestep)
	{
		//Euler integration
		hostProps.linearVelocity += hostProps.linearAcceleration * timestep + hostProps.linearDrag * -hostProps.linearVelocity * timestep;
		deviceProps.linearPosition += hostProps.linearVelocity * timestep;

		hostProps.angularVelocity += hostProps.angularAcceleration * timestep + hostProps.angularDrag * -hostProps.angularVelocity * timestep;
		deviceProps.angularPosition += hostProps.angularVelocity;

		hostProps.lifetimeRemaining -= timestep;

		float lifetime_t = 1.0f - (hostProps.lifetimeRemaining / hostProps.lifetime);

		deviceProps.currentColour = glm::mix(hostProps.startColour, hostProps.endColour, lifetime_t);
		deviceProps.currentSize = glm::mix(hostProps.startSize, hostProps.endSize, lifetime_t);
	}*/

}