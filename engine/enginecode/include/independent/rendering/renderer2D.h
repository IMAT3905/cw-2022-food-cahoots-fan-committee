/** \file renderer2D.h */
#pragma once

#include "rendererCommon.h"

#include "ft2build.h"
#include "freetype/freetype.h"
#include <stdlib.h>
#include <array>

namespace Engine
{
	/** \class Renderer2DVertex
	** \brief Class which holds the layout and data for rendering 2D shapes 
	*/
	class Renderer2DVertex
	{
	public:
		Renderer2DVertex() = default; //!< Default constructor 
		Renderer2DVertex(const glm::vec4& pos, const glm::vec2& UVs, const uint32_t tu, const glm::vec4 pTint) : position(pos), uvCoords(UVs), texUnit(tu), tint(pack(pTint)) {} //!< Constructor \param pos is the position \param UVs is the texture's size \param tu is the texture unit \param pTint is the packed tint of the rendered shape
		glm::vec4 position; //!< position of the rendered shape
		glm::vec2 uvCoords; //!< uv coords of the rendered shape
		uint32_t texUnit; //!< the texture unit used
		uint32_t tint; //!< tint to be applied to the shape
		static VertexBufferLayout layout; //!< data layout
		static uint32_t pack(const glm::vec4& tint); //!< function to pack tint data \param tint the data that will be packed
	};

	/** \class Quad
	** \brief Class for drawing quads using half extents
	*/
	class Quad
	{
	public:
		Quad() = default; //!< Default constructor
		static Quad createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents); //!< Creating a quad using half extents data \param centre is the centre of the quad \param halfExtents is data that represents the distance from the centre to the edges of the shape
		static Quad createTopLeftSize(const glm::vec2& topleft, const glm::vec2& size);
	private:
		glm::vec3 m_translate = glm::vec3(0.f); //!< Translation vector
		glm::vec3 m_scale = glm::vec3(1.0f); //!< Scale vector
		friend class Renderer2D; //!< When Renderer2D is initialised Quad must also be initialised
	};

	/** \class Renderer2D
	** \brief Class which allows the rendering of simple 2D primatives
	*/
	class Renderer2D
	{
	public:
		static void init(); //!< Init the internal data of the renderer
		static void begin(const SceneWideUniforms& swu); //!< Begin a 2D scene \param swu is the scene wide uniform data to be used
		static void submit(const Quad& quad, const glm::vec4& tint); //!< Render a tinted quad \param quad is the target of the rendering \param tint is the tint that will be applied to the quad
		static void submit(const Quad& quad, const SubTexture& texture); //!< Render a textured quad \param quad is the target of the rendering \param texture is the texture to be applied to the quad
		static void submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture); //!< Render a textured and tinted quad \param quad is the target of the rendering \param tint is the tint that will be applied to the quad \param texture is the texture to be applied to the quad
		static void submit(const Quad& quad, const glm::vec4& tint, float angle, bool degrees = false); //!< Render a tinted quad with an angle \param quad is the target of the rendering \param tint is the tint that will be applied to the quad \param angle is the angle of the quad \param degrees is a check to see if the value is in degrees or radians
		static void submit(const Quad& quad, const SubTexture& texture, float angle, bool degrees = false); //!< Render a textured quad with an angle \param quad is the target of the rendering \param texture is the texture to be applied to the quad \param angle is the angle of the quad \param degrees is a check to see if the value is in degrees or radians
		static void submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture, float angle, bool degrees = false); //!< Render a textured and tinted quad with an angle \param quad is the target of the rendering \param tint is the tint that will be applied to the quad \param texture is the texture to be applied to the quad \param angle is the angle of the quad \param degrees is a check to see if the value is in degrees or radians

		static void submit(char ch, const glm::vec2& position, float& advance, const glm::vec4& tint); //!< Render a single character with a tint \param ch is the character to be rendered \param position is the position of the character \param advance is the advance of the character \param tint is the tint to be applied to the character
		static void submit(const char * text, const glm::vec2& position, const glm::vec4& tint); //!< Render a piece of text with a tint \param text is the collection of chars for the text \param position is the position of the text \param tint is the tint applied to the text

		static void end(); //!< Ending a 2D scene
		static void flush(); //!< Clearing the buffers and draw count

	private:
		/** \struct GlyphData
		** \brief A struct for holding data about glyphs
		*/
		struct GlyphData
		{
			unsigned char ch; //!< Character
			glm::vec2 size; //!< The size of a glyph
			glm::vec2 bearing; //!< The bearing of a glyph
			float advance; //!< The advance for the glyph
			SubTexture subTexture; //!< The subtexture of the glyph
		};

		/** \struct InternalData
		** \brief A struct for all the internal data for drawing a 2D shape
		*/
		struct InternalData
		{
			std::shared_ptr<Texture> defaultTexture; //!< The default texture of the 2D shape
			SubTexture defaultSubTexture; //!< The default subtexture of the 2D shape
			glm::vec4 defaultTint; //!< The default tint of the 2D shape
			std::shared_ptr<Shader> shader; //!< A reference to the shader
			std::shared_ptr<VertexArray> VAO; //!< The vertex array object
			std::shared_ptr<UniformBuffer> UBO; //!< The uniform buffer object
			glm::mat4 model; //!< The model 
			std::array<glm::vec4, 4> quad; //!< The quad
			static const uint32_t batchSize = 8192; //!< The total batch size
			uint32_t drawCount; //!< The draw count
			std::vector<Renderer2DVertex> vertices; //!< A vector of vertices
			std::array<int32_t, 32> textureUnits; //!< An array of texture units

			FT_Library ft; //!< Access to freetype
			FT_Face fontFace; //!< Access to fontface
			unsigned char firstGlyph = 32; //!< The first character glyph's position
			unsigned char lastGlyph = 126; //!< The last character glyph's position
			std::vector<GlyphData> glyphData; //!< Access to glyph data
			TextureAtlas glyphAtlas; //!< The atlas for the glyphs
		};

		static std::shared_ptr<InternalData> s_data; //!< All the data from internal data stored

		static void RtoRGBA(unsigned char* DSTbuffer, unsigned char* SRCbuffer, uint32_t width, uint32_t height); //!< Convert to RGBA channels \param DSTbuffer is the destination buffer \param SRCbuffer is the source buffer \param width is the width of the character \param height is the height of the character
	};
}