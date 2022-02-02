/* \file subTexture.h */

#pragma once

#include "rendering/texture.h"
#include <memory>
#include <glm/glm.hpp>

namespace Engine
{
	/** \class SubTexture
	** \brief A class implement subtextures
	*/
	class SubTexture
	{
	public:
		SubTexture() {}; //!< Default constructor
		SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& UVStart, const glm::vec2& UVEnd); //!< Constructor to create a subtexture \param texture is the regular texture \param UVStart is the starting coords of the UV \param UVEnd is the ending coords of the UV
		inline glm::vec2 getUVStart() const { return m_UVStart; } //!< Returns the start coords of the UV
		inline glm::vec2 getUVEnd() const { return m_UVEnd; } //!< Returns the end coords of the UV
		glm::ivec2 getSize() const { return m_size; } //!< Returns size
		glm::vec2 getSizef() const { return { static_cast<float>(m_size.x), static_cast<float>(m_size.y) }; } //!< Returns size as a float
		inline uint32_t getWidth() const { return m_size.x; } //!< Returns Width
		inline uint32_t getHeight() const { return m_size.y; } //!< Returns Height
		inline uint32_t getWidthf() const { return static_cast<float>(m_size.x); } //!< Returns Width as a float
		inline uint32_t getHeightf() const { return static_cast<float>(m_size.y); } //!< Returns Height as a float
		float transformU(float U); //!< Transform U to atlased co-ords \param U is the x axis for UV coords
		float transformV(float V); //!< Transform V to atlased co-ords \param V is the y axis for UV coords
		glm::vec2 transformUV(glm::vec2 UV); //!< Transform UV to atlased co-ords \param UV is the UV coords passed in
		inline const std::shared_ptr<Texture>& getBaseTexture() const { return m_texture; } //!< Returns the base texture
	private:
		std::shared_ptr<Texture> m_texture; //!< The texture
		glm::vec2 m_UVStart; //!< The start of the UV coords
		glm::vec2 m_UVEnd; //!< The end of the UV coords
		glm::ivec2 m_size; //!< Size in pixels
	};
}