/* \file OpenGLFrameBuffer.h */
#pragma once

#include "rendering/frameBuffer.h"
#include "rendering/renderBuffer.h"

namespace Engine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer() { m_ID = 0; }; //!< Default framebuffer
		OpenGLFrameBuffer(glm::ivec2 size, FrameBufferLayout layout);
		~OpenGLFrameBuffer() override;
		void use() override; //!< Bind this framebuffer
		void onResize(WindowResizeEvent& e) override {}; //!< On resize function
		uint32_t getID() override { return m_ID; } //!< Get the API specific render ID
		std::shared_ptr<Texture> getTarget(uint32_t index) override;
	protected:
		uint32_t m_ID;
	};
}