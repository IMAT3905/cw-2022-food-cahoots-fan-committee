/* \file renderAPI.cpp */

#include "engine_pch.h"
#include "rendering/renderAPI.h"
#include "systems/log.h"

#include "platform/OpenGL/OpenGLIndexBuffer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"
#include "platform/OpenGL/OpenGLShader.h"
#include "platform/OpenGL/OpenGLTexture.h"
#include "platform/OpenGL/OpenGLUniformBuffer.h"
#include "platform/OpenGL/OpenGLIndirectBuffer.h"

namespace Engine
{
	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None :
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}

	VertexArray* VertexArray::create()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexArray();
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}

	VertexBuffer* VertexBuffer::create(void* vertices, uint32_t size, const VertexBufferLayout& layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size, layout);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}

	Shader* Shader::createMult(const char* vertexFilepath, const char* fragmentFilepath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(vertexFilepath, fragmentFilepath);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}

	Shader* Shader::create(const char* filepath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(filepath);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}

	Texture* Texture::create(const char* filepath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTexture(filepath);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}

	Texture* Texture::create(uint32_t width, uint32_t height, uint32_t channels, unsigned char* data)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTexture(width, height, channels, data);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}

	UniformBuffer* UniformBuffer::create(const UniformBufferLayout& layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLUniformBuffer(layout);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}

	IndirectBuffer* IndirectBuffer::create(DrawElementsIndirectCommand* commands, uint32_t count)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			Log::error("Not having a rendering API is not currently supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLIndirectBuffer(commands, count);
		case RenderAPI::API::Direct3D:
			Log::error("Direct3D is not currently supported");
			break;
		case RenderAPI::API::Vulkan:
			Log::error("Vulkan is not currently supported");
			break;
		}

		return nullptr;
	}
}
