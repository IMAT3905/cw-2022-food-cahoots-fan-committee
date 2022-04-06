/* \file OpenGLRenderBuffer.h */
#pragma once

#include "rendering/renderBuffer.h"

namespace Engine
{
	class OpenGLRenderBuffer : public RenderBuffer
	{
	public:
		OpenGLRenderBuffer(AttachmentType type, glm::ivec2 size);
		uint32_t getID() override { return m_ID; } //!< Get the API specific render ID
	protected:
		uint32_t m_ID;
	};
}