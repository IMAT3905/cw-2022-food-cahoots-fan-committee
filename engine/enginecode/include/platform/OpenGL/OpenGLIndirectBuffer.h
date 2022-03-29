/* \file OpenGLIndirectBuffer.h */
#pragma once

#include "rendering/indirectBuffer.h"

namespace Engine
{
	class OpenGLIndirectBuffer : public IndirectBuffer
	{
	public:
		OpenGLIndirectBuffer(DrawElementsIndirectCommand* commands, uint32_t count);
		virtual ~OpenGLIndirectBuffer();
		virtual void edit(DrawElementsIndirectCommand* commands, uint32_t size, uint32_t offset) override;
		virtual inline uint32_t getID() const override { return m_OpenGL_ID; } //!< function to return render ID
		virtual inline uint32_t getCount() const override { return m_count; } //!< function to return the count
	private:
		uint32_t m_OpenGL_ID;
		uint32_t m_count;
	};
}