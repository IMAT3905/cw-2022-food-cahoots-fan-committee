/** \file renderer3D.h */
#pragma once

#include "rendererCommon.h"

namespace Engine
{
	/** \class Material
	** \brief Hold a shader and the uniform data associated with that shader
	*/
	class Material
	{
	public:
		Material(const std::shared_ptr<Shader>& shader) : m_shader(shader), m_flags(0), m_texture(nullptr), m_tint(glm::vec4(0.f)) {} //!< A constructor that only requires a shader \param shader is the shader that will be used
		Material(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture, const glm::vec4& tint) : m_shader(shader), m_texture(texture), m_tint(tint) { setFlag(flag_texture | flag_tint); } //!< A constructor that requires a shader, texture and tint \param shader which is the shader that will be used \param texture which will be applied to the object \param tint which will be applied to the object
		Material(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture) : m_shader(shader), m_texture(texture), m_tint(glm::vec4(0.f)) { setFlag(flag_texture); } //!< A constructor that requires a shader and a texture \param shader is the shader that will be used \param texture is the texture that will be applied
		Material(const std::shared_ptr<Shader>& shader, const glm::vec4& tint) : m_shader(shader), m_texture(nullptr), m_tint(tint) { setFlag(flag_tint); } //!< A constructor that requires a shader and a tint \param shader is the shader that will be used \param tint is the tint that will be applied

		inline std::shared_ptr<Shader> getShader() const { return m_shader; } //!< Return the shader
		inline std::shared_ptr<Texture> getTexture() const { return m_texture; } //!< Return the texture
		inline glm::vec4 getTint() const { return m_tint; } //!< Return the tint
		bool isFlagSet(uint32_t flag) const { return m_flags & flag; } //!< Check if a flag is set \param flag is the flag that will be set

		void setTexture(const std::shared_ptr<Texture>& texture) { m_texture = texture; } //!< Function to set a texture \param texture is the texture that will be set
		void setTint(const glm::vec4& tint) { m_tint = tint; } //!< Function to set a tint \param tint is the tint that will be set

		constexpr static uint32_t flag_texture = 1 << 0;	//!< 00000001
		constexpr static uint32_t flag_tint = 1 << 1;		//!< 00000010
	private:
		uint32_t m_flags = 0; //!< Bitfield representation of the shader settings
		std::shared_ptr<Shader> m_shader; //!< The material's shader
		std::shared_ptr<Texture> m_texture; //!< The texture to be applied to the material
		glm::vec4 m_tint; //!< Colour tint to be applied to some geometry
		void setFlag(uint32_t flag) { m_flags = m_flags | flag; } //!< Function to set flags \param flag is the flag that will be set
	};

	/** \class Renderer3D
	** \brief A class which renders 3D geomatry instantly (non-batched)
	*/
	class Renderer3D
	{
	public:
		static void init(); //!< Initialise the renderer
		static void begin(const SceneWideUniforms& sceneWideUniforms); //!< Begin a new 3D scene \param sceneWideUniforms is the scene wide data 
		static void submit(const std::shared_ptr<VertexArray>& geometry, const std::shared_ptr<Material>& material, const glm::mat4& model); //!< Submit a piece of geometry to be rendered \param geometry is the geometry that will be rendered \param material is the object's material \param model is the model number that will be drawn
		static void end(); //!< End the current 3D scene
	private:
		/** \struct InternalData
		** \brief A struct for all the internal data for drawing a 3D shape
		*/
		struct InternalData
		{
			SceneWideUniforms sceneWideUniforms; //!< Stores the scene wide uniforms
			std::shared_ptr<Texture> defaultTexture; //!< Empty white texture
			glm::vec4 defaultTint; //!< Default white tint
		};

		static std::shared_ptr<InternalData> s_data; //!< Data internal to the renderer
	};
}