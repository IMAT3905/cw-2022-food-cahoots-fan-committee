/* \file OpenGLTexture.h */

#pragma once

#include <cstdint>

#include "rendering/texture.h"

namespace Engine
{
	/** \class OpenGLTexture
	** \brief A class for loading textures
	*/
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const char* filepath); //!< Constructor for filepath \param filepath is the location of the texture
		OpenGLTexture(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data); //!< Constructor \param width is the width of the texture \param height is the height of the texture \param channels is the number of channels \param data is the texture data
		virtual ~OpenGLTexture(); //!< Destructor
		virtual void edit(uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, unsigned char* data) override; //!< Function for editting \param xOffset is the offset in the x axis \param yOffset is the offset in the y axis \param width is the width of the texture \param height is the height of the texture \param data is the texture data
		virtual inline uint32_t getID() override { return m_OpenGL_ID; } //!< Returns Render ID
		virtual inline uint32_t getWidth() override { return m_width; } //!< Returns Width
		virtual inline uint32_t getHeight() override { return m_height; } //!< Returns Height
		virtual inline uint32_t getWidthf() override { return static_cast<float>(m_width); }//!< Returns Width
		virtual inline uint32_t getHeightf() override { return static_cast<float>(m_height); } //!< Returns Height
		virtual inline uint32_t getChannels() override { return m_channels; } //!< Returns Number of Channels
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		uint32_t m_width; //!< Image Width
		uint32_t m_height; //!< Image Height
		uint32_t m_channels; //!< Image Channels
		void init(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data); //!< Function for Initialising Values \param width is the width that has been passed in \param height is the height that has been passed in \param channels is the number of channels passed in \param data is the texture data that has been passed in
	};
}