/* \file OpenGLIndirectBuffer.cpp */

#include "engine_pch.h"
#include <glad/glad.h>
#include "platform/OpenGL/OpenGLIndirectBuffer.h"

namespace Engine 
{
	OpenGLIndirectBuffer::OpenGLIndirectBuffer(DrawElementsIndirectCommand* commands, uint32_t count) : m_count(count)
	{
		glCreateBuffers(1, &m_OpenGL_ID);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_OpenGL_ID);
		glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(uint32_t) * count, commands, GL_STATIC_DRAW);
	}

	OpenGLIndirectBuffer::~OpenGLIndirectBuffer()
	{
		glDeleteBuffers(1, &m_OpenGL_ID);
	}

	void OpenGLIndirectBuffer::edit(DrawElementsIndirectCommand* commands, uint32_t size, uint32_t offset)
	{
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_OpenGL_ID);
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offset, size, commands);
	}
}