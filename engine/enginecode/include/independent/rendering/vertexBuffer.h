/* \file vertexBuffer.h */
#pragma once

#include <cstdint>

namespace Engine
{
	/** \class VertexBuffer
	** \brief API agnostic vertex buffer
	*/
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default; //!< Virtual Destructor
		virtual void edit(void* vertices, uint32_t size, uint32_t offset) = 0; //!< function to edit \param vertices is the new number of vertices \param size is the new size fo the vertex buffer \param offset is the new offset
		virtual inline uint32_t getRenderID() const = 0; //!< function to return render ID
		virtual inline const VertexBufferLayout& getLayout() const = 0;//!< function to return layout
		static VertexBuffer* create(void* vertices, uint32_t size, const VertexBufferLayout& layout); //!< function to create a vertex buffer \param vertices is the vertices in the vertex buffer \size is the size of the vertex buffer \param layout is the data layout for the buffer
	};
}