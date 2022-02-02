/* textureAtlas.h */
#pragma once

#include <vector>
#include "rendering/subTexture.h"

namespace Engine
{
	/** \struct SimpleRect
	** \brief Simple rectangle with x, y, width and height parameters
	*/
	struct SimpleRect { int32_t x, y, w, h; };

	/** \class TextureAtlas
	** \brief A class to create texture atlases
	*/
	class TextureAtlas
	{
	public:
		TextureAtlas(glm::ivec2 size = { 4096, 4096 }, uint32_t channels = 4, uint32_t reservedSpace = 32); //!< Constructor \param size is the size of the texture atlas \param channels is the number of channels the textures will have \param reservedSpace is the total number of textures that will have reserved space
		bool add(const char* filepath, SubTexture& result); //!< Add a subtexture \param filepath is the location of the texture \param result is the texture that will be added to the texture atlas
		bool add(uint32_t width, uint32_t height, uint32_t channels, unsigned char * data, SubTexture& result); //!< Add a subtexture \param width is the width of the texture \param height is the height of the texture \param channels is the number of channels \param data is added to the texture atlas \param result is the texture to be added to the texture atlas
		inline uint32_t getChannels() const { return m_baseTexture->getChannels(); } //!< Returns the number of channels
		inline std::shared_ptr<Texture> getBaseTexture() const { return m_baseTexture; } //!< Returns the base texture
		inline uint32_t getID() const { return m_baseTexture->getID(); } //!< Returns the texture's ID
	private:
		std::vector<SimpleRect> m_spaces; //!< Gaps left which can be filled
		std::shared_ptr<Texture> m_baseTexture; //!< Texture which holds all subtexture pixel data
	};
}