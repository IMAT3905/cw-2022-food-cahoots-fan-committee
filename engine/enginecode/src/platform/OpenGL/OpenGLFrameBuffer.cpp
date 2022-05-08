/* \file OpenGLFrameBuffer.cpp */
#include "engine_pch.h"
#include <glad/glad.h>
#include "platform/OpenGL/OpenGLFrameBuffer.h"
#include "rendering/renderBuffer.h"
#include "systems/log.h"

namespace Engine
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(glm::ivec2 size, FrameBufferLayout layout)
	{
		m_size = size;
		m_layout = layout;

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		uint32_t colourAttachmentCount = 0;
		for (auto& [type, isSampled] : m_layout)
		{
			if (isSampled)
			{
				switch (type)
				{
				case AttachmentType::Colour:
					m_sampledTargets.push_back(std::shared_ptr<Texture>(Texture::create(m_size.x, m_size.y, 3, nullptr)));
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colourAttachmentCount, GL_TEXTURE_2D, m_sampledTargets.back()->getID(), 0);
					colourAttachmentCount++;
					break;
				case AttachmentType::Depth:
					m_sampledTargets.push_back(std::shared_ptr<Texture>(Texture::create(m_size.x, m_size.y, 2, nullptr)));
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_sampledTargets.back()->getID(), 0);
					break;
				case AttachmentType::DepthAndStencil:
					m_sampledTargets.push_back(std::shared_ptr<Texture>(Texture::create(m_size.x, m_size.y, 2, nullptr)));
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_sampledTargets.back()->getID(), 0);
					break;
				}
			}
			else
			{
				switch (type)
				{
				case AttachmentType::Colour:
					m_nonSampledTargets.push_back(std::shared_ptr<RenderBuffer>(RenderBuffer::create(type, size)));
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colourAttachmentCount, GL_RENDERBUFFER, m_nonSampledTargets.back()->getID(), 0);
					break;
				case AttachmentType::Depth:
					m_nonSampledTargets.push_back(std::shared_ptr<RenderBuffer>(RenderBuffer::create(type, size)));
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_nonSampledTargets.back()->getID());
					break;
				case AttachmentType::DepthAndStencil:
					m_nonSampledTargets.push_back(std::shared_ptr<RenderBuffer>(RenderBuffer::create(type, size)));
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_nonSampledTargets.back()->getID());
					break;
				}
			}
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Log::error("Framebuffer is not complete");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		m_sampledTargets.clear();
		glDeleteFramebuffers(1, &m_ID);
	}

	void OpenGLFrameBuffer::use()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	std::shared_ptr<Texture> OpenGLFrameBuffer::getTarget(uint32_t index)
	{
		std::shared_ptr<Texture> result = nullptr;

		if (index < m_sampledTargets.size()) result = m_sampledTargets[index];

		return result;
	}
}