/* \file OpenGLVertextBuffer.h */
#pragma once

#include "rendering/bufferLayout.h"
#include "rendering/vertexBuffer.h"

namespace Engine
{
	/** \class OpenGLVertexBuffer
	** \brief A class for creating a vertex buffer
	*/
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* vertices, uint32_t size, VertexBufferLayout layout); //!< Constructor \param vertices is the vertex data \param size is the number of vertices \param layout is the layout of the data in the buffer
		virtual ~OpenGLVertexBuffer(); //!< Destructor
		virtual void edit(void* vertices, uint32_t size, uint32_t offset) override; //!< function to edit \param vertices is the vertex data \param size is the number of vertices \param offset is the offset of the data
		virtual inline uint32_t getRenderID() const override { return m_OpenGL_ID; } //!< function to return render ID
		virtual inline const VertexBufferLayout& getLayout() const override { return m_layout; } //!< function to return layout
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		VertexBufferLayout m_layout; //!< buffer layout
	};
}
