#pragma once

#include "rendererCommon.h"
#include "systems/randomiser.h"

namespace Engine
{
	struct ParticleHostProprties
	{
		glm::vec3 linearVelocity = { 0.f, 0.f, 0.f };
		glm::vec3 linearAcceleration = { 0.f, 0.f, 0.f };
		glm::vec3 linearDrag = { 0.f, 0.f, 0.f };
		float angularVelocity = 0.f;
		float angluarAcceleration = 0.f;
		float angularDrag = 0.f;
		float lifetime = 0.f;
		float lifetimeRemaining = 0.f;
		glm::vec4 startColour = { 0.f, 1.f, 1.f, 1.f };
		glm::vec4 endColour = { 0.f, 0.f, 0.f, 0.f };
		glm::vec2 startSize = { 1.f, 1.f };
		glm::vec2 endSize = { 1.f, 1.f };
		glm::vec3 positionRandomisation = { 0.f, 0.f, 0.f };
		glm::vec3 velocityRandomisation = { 0.f, 0.f, 0.f };
		float scaleRandomisation = 0.f;
		glm::vec3 colourRandomisation = { 0.f, 0.f, 0.f };
		RandomTypes positionRandomType = RandomTypes::None;
		RandomTypes velocityRandomType = RandomTypes::None;
		RandomTypes scaleRandomType = RandomTypes::None;
		RandomTypes colourRandomType = RandomTypes::None;
	};

	struct particleDeviceProperties
	{
		glm::vec3 linearPosition = { 0.f, 0.f, 0.f };
		float angularPosition = 0.f;
		glm::vec4 currentColour;
		glm::vec2 currentSize;
		glm::vec2 current_UVStart;
		glm::vec2 current_UVEnd;
	};

	enum class BlendModes{None, Mix, Additive};
	class Particle
	{
	public:
		Particle::Particle(ParticleHostProprties& Hprops, particleDeviceProperties& Dprops, BlendModes p_blendMode = BlendModes::None);
		void onUpdate(float timestep);
		ParticleHostProprties hostProps;
		particleDeviceProperties deviceProps;
		bool destroyMe() { return hostProps.lifetimeRemaining <= 0.f; }
		BlendModes blendMode;
	};

	class ParticleVertex
	{
	public:
		ParticleVertex() = default;
		ParticleVertex(const glm::vec2& pos, const glm::vec3& pCentre, const glm::vec2& UVs, uint32_t tu, const glm::vec4& pTint) :
			position(pos), centre(pCentre), uvCoords(UVs), texUnit(tu), tint(RendererCommon::pack(pTint)) {}
		glm::vec2 position;
		glm::vec3 centre;
		glm::vec2 uvCoords;
		uint32_t texUnit;
		uint32_t tint;
		static VertexBufferLayout layout;
	};

	class ParticleSystem
	{
	public:
		static void init(uint32_t particleCapacity);
		static void addParticle(Particle& p);
		static void onUpdate(float timestep, glm::vec3& camera);
		static void onRender(const SceneWideUniforms& swu);
		static bool addTexture(const char* filepath, uint32_t& index);
		static bool getUVs(uint32_t index, glm::vec2& UV_start, glm::vec2& UV_end);
	private:
		static void addParticleRenderData(Particle& p);

		struct InternalData
		{
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexArray> VAO;
			std::shared_ptr<UniformBuffer> UBO;
			std::array<glm::vec4, 4> quad;
			uint32_t particleBatchSize;
			uint32_t particleCapacity;
			uint32_t nbParticleDrawCount;
			uint32_t mbParticleDrawCount;
			uint32_t abParticleDrawCount;
			std::vector<ParticleVertex> nonBlendedVertices;
			std::vector<ParticleVertex> mixBlendedVertices;
			std::vector<ParticleVertex> additiveBlendedVertices;
			std::vector<Particle> nonBlendedParticles;
			std::vector<Particle> mixBlendedParticles;
			std::vector<Particle> additiveBlendedParticles;
			std::shared_ptr<TextureAtlas> atlas;
			std::vector<SubTexture> particleTextures;
			uint32_t textureUnit;
		};

		static std::shared_ptr<InternalData> s_data;
	};
}