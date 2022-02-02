/* \file vertexArray.h */
#pragma once

#include <cstdint>

#include "include/platform/OpenGL/OpenGLVertexBuffer.h"
#include "include/platform/OpenGL/OpenGLIndexBuffer.h"

namespace Engine
{
	/** \class VertexArray
	** \brief API agnostic vertex array
	*/
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default; //!< Destructor
		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0; //!< function for adding a vertex buffer \param vertexBuffer is the vertex buffer that will be added
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0; //!< function for setting an index buffer \param indexBuffer is the index buffer that will be set
		virtual inline uint32_t getID() const = 0; //!< function to return render ID
		virtual inline uint32_t getDrawCount() const = 0; //!< function for returning the draw count
		virtual std::vector<std::shared_ptr<VertexBuffer>> getVertexBuffers() const = 0; //!< function for returning the amount of vertex buffers
		virtual std::shared_ptr<IndexBuffer> getIndexBuffer() const = 0; //!< function for returning the amount of vertex buffers

		static VertexArray* create(); //!< function to create an index buffer
	};
}