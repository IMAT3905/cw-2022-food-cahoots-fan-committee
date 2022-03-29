/** \file renderer3D.h */
#pragma once

#include "rendererCommon.h"

namespace Engine
{
	/** \class Renderer3DVertex
	** \brief Class which uses a textured phong and has normalised data
	*/
	class Renderer3DVertex
	{
	public:
		Renderer3DVertex() = default; //!< Default constructor
		Renderer3DVertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& uv) : m_pos(pos), m_normal(normal), m_uv(uv) {} //!< Constructor \param pos is the position of the object \param normal is the normal data for the object \param uv is the uv data for the object
		glm::vec3 m_pos; //!< The position data
		glm::vec3 m_normal; //!< The normal data
		glm::vec2 m_uv; //!< The UV data
		static VertexBufferLayout layout; //!< The layout of the data
		//std::array<int16_t, 3> normalise(const glm::vec3& normal);
		//std::array<int16_t, 2> normalise(const glm::vec2& uv);
	};


	/** \class Material
	** \brief Hold a shader and the uniform data associated with that shader
	*/
	class Material
	{
	public:
		Material(const std::shared_ptr<Shader>& shader) : m_shader(shader), m_flags(0), m_texture(nullptr), m_tint(glm::vec4(0.f)) {} //!< A constructor that only requires a shader \param shader is the shader that will be used
		Material(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture, const glm::vec4& tint) : m_shader(shader), m_texture(texture), m_tint(tint) { setFlag(flag_texture | flag_tint); } //!< A constructor that requires a shader, texture and tint \param shader which is the shader that will be used \param texture which will be applied to the object \param tint which will be applied to the object
		Material(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture) : m_shader(shader), m_texture(texture), m_tint(glm::vec4(0.f)) { setFlag(flag_texture); } //!< A constructor that requires a shader and a texture \param shader is the shader that will be used \param texture is the texture that will be applied
		Material(const std::shared_ptr<Shader>& shader, const glm::vec4& tint) : m_shader(shader), m_texture(RendererCommon::defaultTexture), m_tint(tint) { setFlag(flag_tint); } //!< A constructor that requires a shader and a tint \param shader is the shader that will be used \param tint is the tint that will be applied

		inline std::shared_ptr<Shader> getShader() const { return m_shader; } //!< Return the shader
		inline std::shared_ptr<Texture> getTexture() const { return m_texture; } //!< Return the texture
		inline glm::vec4 getTint() const { return m_tint; } //!< Return the tint
		bool isFlagSet(uint32_t flag) const { return m_flags & flag; } //!< Check if a flag is set \param flag is the flag that will be set

		void setTexture(const std::shared_ptr<Texture>& texture) { m_texture = texture; } //!< Function to set a texture \param texture is the texture that will be set
		void setTint(const glm::vec4& tint) { m_tint = tint; } //!< Function to set a tint \param tint is the tint that will be set

		constexpr static uint32_t flag_batched = 1 << 0;	//!< 00000001
		constexpr static uint32_t flag_texture = 1 << 1;	//!< 00000010
		constexpr static uint32_t flag_tint = 1 << 2;		//!< 00000100
		
	private:
		uint32_t m_flags = 0; //!< Bitfield representation of the shader settings
		std::shared_ptr<Shader> m_shader; //!< The material's shader
		std::shared_ptr<Texture> m_texture; //!< The texture to be applied to the material
		glm::vec4 m_tint; //!< Colour tint to be applied to some geometry
		void setFlag(uint32_t flag) { m_flags = m_flags | flag; } //!< Function to set flags \param flag is the flag that will be set
	};

	struct Geometry
	{
		uint32_t id; //!< Unique ID and position in commands
		uint32_t vertexCount; //!< Number of vertices
		uint32_t indexCount; //!< Number of indices - effectively a draw count
		uint32_t firstVertex; //!< Index in the VBO of the first vertex of this geometry
		uint32_t firstIndex; //!< Index in the IBO of the first indices of this geometry
	};

	struct BatchQueueEntry
	{
		Geometry geometry;
		std::shared_ptr<Material> material;
		glm::mat4 model;
	};

	/** \class Renderer3D
	** \brief A class which renders 3D geometry instantly
	*/
	class Renderer3D
	{
	public:
		static void init(uint32_t vertexCapacity, uint32_t indexCapacity, uint32_t batchSize); //!< Initialise the renderer
		static void begin(const SceneWideUniforms& sceneWideUniforms); //!< Begin a new 3D scene \param sceneWideUniforms is the scene wide data 
		static void submit(const Geometry& geometry, const std::shared_ptr<Material>& material, const glm::mat4& model); //!< Submit a piece of geometry to be rendered \param geometry is the geometry that will be rendered \param material is the object's material \param model is the model number that will be drawn
		static void end(); //!< End the current 3D scene
		static void flush(); //!< flush all draw queues

		static void initShader(std::shared_ptr<Shader> shader); //!< connect shader to the renderer
		static bool addGeometry(std::vector<Renderer3DVertex> vertices, std::vector<uint32_t> indices, Geometry& geo);
	private:
		static void flushBatch(); //!< flush batch draw queues
		static void flushBatchCommands(std::shared_ptr<Shader>& shader, uint32_t instanceCount);

		/** \struct InternalData
		** \brief A struct for all the internal data for drawing a 3D shape
		*/
		struct InternalData
		{
			std::shared_ptr<UniformBuffer> cameraUBO; //!< View and proj mats
			std::shared_ptr<UniformBuffer> lightsUBO; //!< Scenewide lighting variables
			std::shared_ptr<VertexArray> VAO; //!< All static meshes
			std::shared_ptr<IndirectBuffer> commands; //!< Command buffer
			std::vector<BatchQueueEntry> batchQueue; //!< Queue waiting to be drawn in batches
			std::vector<DrawElementsIndirectCommand> batchCommands;
			uint32_t batchCapacity = 0;
			uint32_t vertexCapacity = 0;
			uint32_t indexCapacity = 0;
			uint32_t nextVertex = 0;
			uint32_t nextIndex = 0;

			std::vector<glm::mat4> modelInstanceData;
			std::vector<uint32_t> tintInstanceData;
			std::vector<uint32_t> texInstanceData;
		};

		static std::shared_ptr<InternalData> s_data; //!< Data internal to the renderer
	};
}