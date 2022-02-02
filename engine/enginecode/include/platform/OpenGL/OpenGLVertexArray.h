/* \file OpenGLVertexArray */
#pragma once

#include <vector>
#include <memory>
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "rendering/vertexArray.h"

namespace Engine
{
	/** \class OpenGLVertexArray
	** \brief A class for creating a vertex array
	*/
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray(); //!< Constructor
		virtual ~OpenGLVertexArray(); //!< Destructor
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override; //!< function for adding a vertex buffer \param vertexBuffer is the vertex buffer that will be added
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override; //!< function for setting an index buffer \param indexBuffer is the index buffer that will be set
		virtual inline uint32_t getID() const { return m_OpenGL_ID; } //!< function to return render ID
		virtual inline uint32_t getDrawCount() const { if (m_indexBuffer) { m_indexBuffer->getCount(); } else { return 0; } } //!< function for returning the draw count
		virtual std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const { return m_vertexBuffer; } //!< function for returning number of vertex buffers
		virtual std::shared_ptr<IndexBuffer> getIndexBuffer() const { return m_indexBuffer; } //!< function for returning the amount of vertex buffers
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		uint32_t m_attributeIndex = 0; //!< Attribute index
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffer; //!< Vertex buffer
		std::shared_ptr<IndexBuffer> m_indexBuffer; //!< Index buffer
	};
}