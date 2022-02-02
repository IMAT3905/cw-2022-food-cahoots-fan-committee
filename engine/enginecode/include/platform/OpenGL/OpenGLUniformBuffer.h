/** \file OpenGLUniformBuffer.h */
#pragma once

#include "rendering/uniformBuffer.h"

namespace Engine
{
	/** \class OpenGLUniformBuffer
	** \brief A class for creating a uniform buffer
	*/
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const UniformBufferLayout& layout); //!< Constructor \param layout is the layout of the data for the buffer
		~OpenGLUniformBuffer() override; //!< Destructor
		inline uint32_t getID() override { return m_OpenGL_ID; } //!< function to return ID
		inline UniformBufferLayout getLayout() override { return m_layout; } //!< function to return layout
		void attachShaderBlock(const std::shared_ptr<Shader>& shader, const char* blockName) override; //!< function to attach a shader block \param shader is the passed in shader \param blockName is the name of the uniform 
		void uploadData(const char* uniformName, void* data) override; //!< function to upload data \param uniformName is the name of the data uniform \param data is the data that will be uploaded
	private:
		uint32_t m_OpenGL_ID; //!< OpenGL ID
		static uint32_t s_blockNumber; //!< Global block number
	};
}