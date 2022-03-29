/* \file OpenGLIndexBuffer.h */
#pragma once

#include "rendering/indexBuffer.h"

namespace Engine
{
	/** \class OpenGLIndexBuffer
	** \brief A class for an index buffer
	*/
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count); //!< Constructor \param indices is the indices data \param count is the number of indices
		virtual ~OpenGLIndexBuffer(); //!< Destructor
		virtual void edit(void* indices, uint32_t size, uint32_t offset) override;
		virtual inline uint32_t getID() const override { return m_OpenGL_ID; } //!< function to return render ID
		virtual inline uint32_t getCount() const override { return m_count; } //!< function to return the count
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		uint32_t m_count; //!< Effective draw count
	};
}