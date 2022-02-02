/** \file uniformBuffer.h */
#pragma once

#include <ctype.h>
#include <unordered_map>
#include <memory>

#include "bufferLayout.h"
#include "shader.h"

namespace Engine
{
	/** \class UniformBuffer
	** \brief A class for creating a buffer for uniforms
	*/
	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default; //!< Destructor
		virtual uint32_t getID() = 0; //!< function to return ID
		virtual UniformBufferLayout getLayout() = 0; //!< function to return layout
		virtual void attachShaderBlock(const std::shared_ptr<Shader>& shader, const char* blockName) = 0; //!< Function to attach a shader block \param shader is the shader that the block is attached to \param blockName is the name of the block
		virtual void uploadData(const char* uniformName, void* data) = 0; //!< Function to upload data \param uniformName is the name of the uniform that the data is associated with \param data is the data that will be uploaded to the uniform
		static UniformBuffer* create(const UniformBufferLayout& layout); //!< Function to create a uniform buffer \param layout is the data layout for the buffer
	protected:
		UniformBufferLayout m_layout; //!< Uniform buffer layout
		std::unordered_map<const char*, std::pair<uint32_t, uint32_t>> m_uniformCache; //!< Stores uniform names with offsets and sizes
		uint32_t m_blockNumber; //!< Block number for this UBO
	};
}