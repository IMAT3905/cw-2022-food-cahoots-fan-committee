#pragma once

#include "rendererCommon.h"

namespace Engine
{
	class BillboardQuad
	{
	public: 
		BillboardQuad() = default; 
		BillboardQuad(const glm::vec3& centre, const glm::vec2& halfExtents) : m_centre(centre), m_size(halfExtents * 2.0f) {};
	private:
		glm::vec3 m_centre;
		glm::vec2 m_size;
		friend class Renderer2DBillboard;
	};

	class BillboardVertex
	{
	public:
		BillboardVertex() = default;
		BillboardVertex(const glm::vec2& pos, const glm::vec3& pCentre, const glm::vec2& UVs, uint32_t tu, const glm::vec4& pTint) :
			position(pos), centre(pCentre), uvCoords(UVs), texUnit(tu), tint(RendererCommon::pack(pTint)) {}
		glm::vec2 position;
		glm::vec3 centre;
		glm::vec2 uvCoords;
		uint32_t texUnit;
		uint32_t tint;
		static VertexBufferLayout layout;
	};

	class Renderer2DBillboard
	{
	public:
		static void init(uint32_t batchSize);
		static void submit(const BillboardQuad& quad, const glm::vec4& tint, const SubTexture& texture);
		static void begin(const SceneWideUniforms& swu);
		static void end();
		static void flush();
	private:
		struct InternalData
		{
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexArray> VAO;
			std::shared_ptr<UniformBuffer> UBO;
			std::array<glm::vec4, 4> quad;
			uint32_t batchSize;
			uint32_t drawCount;
			std::vector<BillboardVertex> vertices;
		};

		static std::shared_ptr<InternalData> s_data;
	};
}