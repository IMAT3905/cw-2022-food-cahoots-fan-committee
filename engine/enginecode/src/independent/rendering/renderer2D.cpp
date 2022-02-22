/* \file renderer2D.cpp */

#include "engine_pch.h"
#include "systems/log.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "rendering/renderer2D.h"
#include <numeric>

namespace Engine
{
	TextureUnitManager RendererCommon::texUnitMan = TextureUnitManager(32);

	VertexBufferLayout Renderer2DVertex::layout = VertexBufferLayout({ ShaderDataType::Float4, ShaderDataType::Float2, ShaderDataType::FlatInt, { ShaderDataType::Byte4, true } });

	std::shared_ptr<Renderer2D::InternalData> Renderer2D::s_data = nullptr;

	void Renderer2D::init()
	{
		int i = sizeof(Renderer2DVertex);
		s_data.reset(new InternalData);

		unsigned char whitePx[4] = { 255, 255, 255, 255 };
		s_data->defaultTexture.reset(Texture::create(1, 1, 4, whitePx));

		s_data->defaultSubTexture = SubTexture(s_data->defaultTexture, { 0.f, 0.f }, { 1.f, 1.f });

		s_data->defaultTint = { 1.f, 1.f, 1.f, 1.f };

		s_data->model = glm::mat4(1.0f);

		s_data->textureUnits = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

		s_data->shader.reset(Shader::create("./assets/shaders/quad5.glsl"));

		s_data->UBO.reset(UniformBuffer::create(UniformBufferLayout({
			{ "u_projection", ShaderDataType::Mat4 },
			{ "u_view", ShaderDataType::Mat4 }
			})));

		s_data->quad[0] = { -0.5f, -0.5f, 1.f, 1.f };
		s_data->quad[1] = { -0.5f,  0.5f, 1.f, 1.f };
		s_data->quad[2] = { 0.5f,  0.5f, 1.f, 1.f };
		s_data->quad[3] = { 0.5f, -0.5f, 1.f, 1.f };

		s_data->vertices.resize(s_data->batchSize);
		s_data->vertices[0] = Renderer2DVertex(s_data->quad[0], { 0.f, 0.f }, 0, glm::vec4(1.f));
		s_data->vertices[1] = Renderer2DVertex(s_data->quad[1], { 0.f, 1.f }, 0, glm::vec4(1.f));
		s_data->vertices[2] = Renderer2DVertex(s_data->quad[2], { 1.f, 1.f }, 0, glm::vec4(1.f));
		s_data->vertices[3] = Renderer2DVertex(s_data->quad[3], { 1.f, 0.f }, 0, glm::vec4(1.f));

		std::vector<uint32_t> indices(s_data->batchSize);
		std::iota(indices.begin(), indices.end(), 0);

		std::shared_ptr<VertexBuffer> VBO;
		std::shared_ptr<IndexBuffer> IBO;

		s_data->VAO.reset(VertexArray::create());
		VBO.reset(VertexBuffer::create(s_data->vertices.data(), sizeof(Renderer2DVertex) * s_data->vertices.size(), Renderer2DVertex::layout));
		IBO.reset(IndexBuffer::create(indices.data(), indices.size()));
		s_data->VAO->addVertexBuffer(VBO);
		s_data->VAO->setIndexBuffer(IBO);

		s_data->UBO->attachShaderBlock(s_data->shader, "b_camera");

		// File path to a font
		const char* filePath = "./assets/fonts/arial.ttf";

		// Init freetype
		if (FT_Init_FreeType(&s_data->ft)) Log::error("Error: Freetype could not be initialised");

		// Load the font
		if (FT_New_Face(s_data->ft, filePath, 0, &s_data->fontFace)) Log::error("Error: Freetype could not load font: {0}", filePath);

		// Set the char size
		int32_t charSize = 86;
		if (FT_Set_Pixel_Sizes(s_data->fontFace, 0, charSize)) Log::error("Error: Freetype can't set font size {0}", charSize);

		// Fill the texture atlas
		s_data->glyphData.resize(s_data->lastGlyph - s_data->firstGlyph + 1);

		std::vector <std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>> tmpGlyphData;
		tmpGlyphData.resize(s_data->lastGlyph - s_data->firstGlyph + 1);

		for (unsigned char ch = s_data->firstGlyph; ch <= s_data->lastGlyph; ch++)
		{
			// Get gylph from freetype
			if (FT_Load_Char(s_data->fontFace, ch, FT_LOAD_RENDER)) Log::error("Error: Could not load glyph for char {0}", ch);
			else
			{
				auto& pair1 = tmpGlyphData.at(ch - s_data->firstGlyph);
				pair1.first = ch;
				GlyphData& gd = pair1.second.first;

				// Get gylph data
				gd.ch = ch;
				gd.size = glm::vec2(s_data->fontFace->glyph->bitmap.width, s_data->fontFace->glyph->bitmap.rows);
				gd.bearing = glm::vec2(s_data->fontFace->glyph->bitmap_left, -s_data->fontFace->glyph->bitmap_top);
				// Calculate the advance
				gd.advance = static_cast<float>(s_data->fontFace->glyph->advance.x >> 6);

				// Sort glyph texture

				// Set the dim of the glyph buffer
				glm::vec2 glyphBufferDims = gd.size;
				uint32_t glyphBufferSize = glyphBufferDims.x * glyphBufferDims.y * 4 * sizeof(unsigned char);

				pair1.second.second = static_cast<unsigned char*>(malloc(glyphBufferSize));
				RtoRGBA(pair1.second.second, s_data->fontFace->glyph->bitmap.buffer, gd.size.x, gd.size.y);
			}
		}

		std::sort(tmpGlyphData.begin(), tmpGlyphData.end(),
			[](std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>& a, std::pair<unsigned char, std::pair<GlyphData, unsigned char*>>& b)
			{
				return a.second.first.size.x > b.second.first.size.x;
			});

		for (unsigned char currentGlyph = 0; currentGlyph <= s_data->lastGlyph - s_data->firstGlyph; currentGlyph++)
		{
			auto& gd = tmpGlyphData.at(currentGlyph).second.first;
			auto glyphBuffer = tmpGlyphData.at(currentGlyph).second.second;
			unsigned char ch = tmpGlyphData.at(currentGlyph).first;
			gd.subTexture = SubTexture(s_data->glyphAtlas.getBaseTexture(), glm::vec2(0.f), glm::vec2(0.f));
			if (!s_data->glyphAtlas.add(gd.size.x, gd.size.y, 4, glyphBuffer, gd.subTexture))
			{
				Log::error("No space to add glyph '{0}' ", ch);			
			}
			s_data->glyphData.at(currentGlyph) = gd;

			free(glyphBuffer);
		}

		std::sort(s_data->glyphData.begin(), s_data->glyphData.end(),
			[](GlyphData& a, GlyphData& b)
			{
				return a.ch < b.ch;
			});
	}

	void Renderer2D::begin(const SceneWideUniforms& swu)
	{
		// Reset drawcount
		s_data->drawCount = 0;

		// Bind the shader
		glUseProgram(s_data->shader->getID());

		s_data->shader->uploadIntArray("u_texData", s_data->textureUnits.data(), 32);

		glBindBuffer(GL_UNIFORM_BUFFER, s_data->UBO->getID());
		s_data->UBO->uploadData("u_projection", swu.at("u_projection").second);
		s_data->UBO->uploadData("u_view", swu.at("u_view").second);
	}

	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint)
	{
		Renderer2D::submit(quad, tint, s_data->defaultSubTexture);
	}

	void Renderer2D::submit(const Quad& quad, const SubTexture& texture)
	{
		Renderer2D::submit(quad, s_data->defaultTint, texture);
	}

	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture)
	{
		if (s_data->drawCount + 4 > s_data->batchSize) flush();

		uint32_t texUnit;
		if (RendererCommon::texUnitMan.full()) flush();
		if (RendererCommon::texUnitMan.getUnit(texture.getBaseTexture()->getID(), texUnit))
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, texture.getBaseTexture()->getID());
		}

		s_data->model = glm::scale(glm::translate(glm::mat4(1.f), quad.m_translate), quad.m_scale);
		uint32_t packedTint = Renderer2DVertex::pack(tint);
		
		uint32_t startIdx = s_data->drawCount;
		for (int i = 0; i < 4; i++)
		{
			s_data->vertices[i + startIdx].position = s_data->model * s_data->quad[i];
			s_data->vertices[i + startIdx].tint = packedTint;
			s_data->vertices[i + startIdx].texUnit = texUnit;
		}
		s_data->vertices[startIdx + 0].uvCoords = texture.getUVStart();
		s_data->vertices[startIdx + 1].uvCoords = { texture.getUVStart().x, texture.getUVEnd().y };
		s_data->vertices[startIdx + 2].uvCoords = texture.getUVEnd();
		s_data->vertices[startIdx + 3].uvCoords = { texture.getUVEnd().x, texture.getUVStart().y };

		s_data->drawCount += 4;
	}

	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, float angle, bool degrees)
	{
		Renderer2D::submit(quad, tint, s_data->defaultSubTexture, angle, degrees);
	}

	void Renderer2D::submit(const Quad& quad, const SubTexture& texture, float angle, bool degrees)
	{
		Renderer2D::submit(quad, s_data->defaultTint, texture, angle, degrees);
	}

	void Renderer2D::submit(const Quad& quad, const glm::vec4& tint, const SubTexture& texture, float angle, bool degrees)
	{
		if (s_data->drawCount + 4 > s_data->batchSize) flush();

		if (degrees) angle = glm::radians(angle);

		uint32_t texUnit;
		if (RendererCommon::texUnitMan.full()) flush();
		if (RendererCommon::texUnitMan.getUnit(texture.getBaseTexture()->getID(), texUnit));
		{
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, texture.getBaseTexture()->getID());
		}

		s_data->model = glm::scale(glm::rotate(glm::translate(glm::mat4(1.f), quad.m_translate), angle, { 0.f, 0.f, 1.f }), quad.m_scale);
		uint32_t packedTint = Renderer2DVertex::pack(tint);

		uint32_t startIdx = s_data->drawCount;
		for (int i = 0; i < 4; i++)
		{
			s_data->vertices[i + startIdx].position = s_data->model * s_data->quad[i];
			s_data->vertices[i + startIdx].tint = packedTint;
			s_data->vertices[i + startIdx].texUnit = texUnit;
		}
		s_data->vertices[startIdx + 0].uvCoords = texture.getUVStart();
		s_data->vertices[startIdx + 1].uvCoords = { texture.getUVStart().x, texture.getUVEnd().y };
		s_data->vertices[startIdx + 2].uvCoords = texture.getUVEnd();
		s_data->vertices[startIdx + 3].uvCoords = { texture.getUVEnd().x, texture.getUVStart().y };

		s_data->drawCount += 4;
	}

	void Renderer2D::submit(char ch, const glm::vec2& position, float& advance, const glm::vec4& tint)
	{
		// Get glyph data
		GlyphData& gd = s_data->glyphData.at(ch - s_data->firstGlyph);

		// Calculate the quad for the glyph
		glm::vec2 glyphHalfExtents(gd.size * 0.5f);
		glm::vec2 glyphCentre = (position + gd.bearing) + glyphHalfExtents;
		Quad quad = Quad::createCentreHalfExtents(glyphCentre, glyphHalfExtents);

		// Set advance
		advance = gd.advance;

		// Submit quad
		submit(quad, tint, gd.subTexture);
	}

	void Renderer2D::submit(const char* text, const glm::vec2& position, const glm::vec4& tint)
	{
		uint32_t len = strlen(text);
		float advance = 0.f, x = position.x;
		
		for (uint32_t i = 0; i < len; i++)
		{
			submit(text[i], { x, position.y }, advance, tint);
			x += advance;
		}
	}

	void Renderer2D::end()
	{
		if (s_data->drawCount > 0) flush();
	}

	void Renderer2D::flush()
	{
		s_data->VAO->getVertexBuffers().at(0)->edit(s_data->vertices.data(), sizeof(Renderer2DVertex) * s_data->drawCount, 0);

		glBindVertexArray(s_data->VAO->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->VAO->getIndexBuffer()->getID());

		glDrawElements(GL_QUADS, s_data->drawCount, GL_UNSIGNED_INT, nullptr);

		s_data->drawCount = 0;
	}

	void Renderer2D::RtoRGBA(unsigned char* DSTbuffer, unsigned char* SRCbuffer, uint32_t width, uint32_t height)
	{
		unsigned char* pWalker = DSTbuffer;
		for (uint32_t i = 0; i < height; i++)
		{
			for (int32_t j = 0; j < width; j++)
			{
				*pWalker = 255; pWalker++;
				*pWalker = 255; pWalker++;
				*pWalker = 255; pWalker++;
				*pWalker = *SRCbuffer; // Set alpha channel
				pWalker++; // Go to red of next pixel
				SRCbuffer++; // Go to next monochrome pixel
			}
		}
	}

	Quad Quad::createCentreHalfExtents(const glm::vec2& centre, const glm::vec2& halfExtents)
	{
		Quad result;

		result.m_translate = glm::vec3(centre, 0.f);
		result.m_scale = glm::vec3(halfExtents * 2.f, 1.f);

		return result;
	}

	Quad Quad::createTopLeftSize(const glm::vec2& topleft, const glm::vec2& size)
	{
		glm::vec2 halfextents = size * 2.0f;
		glm::vec2 centre = topleft + halfextents;
		return createCentreHalfExtents(centre, halfextents);
	}

	uint32_t Renderer2DVertex::pack(const glm::vec4& tint)
	{
		uint32_t result = 0;

		uint32_t r = (static_cast<uint32_t>(tint.r * 255.f)) << 0;
		uint32_t g = (static_cast<uint32_t>(tint.g * 255.f)) << 8;
		uint32_t b = (static_cast<uint32_t>(tint.b * 255.f)) << 16;
		uint32_t a = (static_cast<uint32_t>(tint.a * 255.f)) << 24;

		result = (r | g | b | a);
		return result;
	}
}