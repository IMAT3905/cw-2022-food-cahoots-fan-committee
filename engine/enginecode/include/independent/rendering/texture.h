/* \file texture.h */
#pragma once

#include <cstdint>

namespace Engine
{
	/** \class Texture
	** \brief API agnostic texture
	*/
	class Texture
	{
	public:
		virtual ~Texture() = default; //!< Destructor
		virtual void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data) = 0; //!< Function for editting a texture \param xOffset is the offset in the x axis \param yOffset is the offset in the y axis \param width is the width of the texture \param height is the height of the texture \param data is the texture data
		virtual inline uint32_t getID() = 0; //!< Returns Render ID
		virtual inline uint32_t getWidth() = 0; //!< Returns Width
		virtual inline uint32_t getHeight() = 0; //!< Returns Height
		virtual inline uint32_t getWidthf() = 0; //!< Returns Width as a float
		virtual inline uint32_t getHeightf() = 0; //!< Returns Height as a float
		virtual inline uint32_t getChannels() = 0; //!< Returns Number of Channels

		static Texture* create(const char* filepath); //!< function to create a texture \param filepath is the location of the texture
		static Texture* create(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data); //!< function to create a texture \param width is the width of the texture \param height is the height of the texture \param channels is the number of channels the texture has \param data is the texture data
	};
}