/* \file indexBuffer.h */
#pragma once

#include <cstdint>

namespace Engine
{
	/** \class IndexBuffer
	** \brief API agnostic index buffer
	*/
	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default; //!< Virtual Destructor
		virtual void edit(void* indices, uint32_t size, uint32_t offset) = 0;
		virtual inline uint32_t getID() const = 0; //!< Function to return render ID
		virtual inline uint32_t getCount() const = 0; //!< Function to return the count

		static IndexBuffer* create(uint32_t* indices, uint32_t count); //!< Function to create an index buffer \param indices is data passed in \param count is the number of indices 
	};
}