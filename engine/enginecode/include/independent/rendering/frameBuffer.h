/* \file frameBuffer.h */
#pragma once

#include "events/events.h"
#include "texture.h"
#include "rendering/rendererCommon.h"
#include "rendering/renderBuffer.h"
#include <vector>

namespace Engine
{
	using Attachment = std::pair<AttachmentType, bool>;

	class FrameBufferLayout
	{
	public:
		FrameBufferLayout() {}; //!< Default constructor
		FrameBufferLayout(const std::initializer_list<Attachment>& attachments, uint32_t stride = 0) : m_attachments(attachments) {}
		void addAttachment(AttachmentType type, bool sampled = false)
		{
			m_attachments.push_back(Attachment(type, sampled));
		}
		std::vector<Attachment>::iterator begin() { return m_attachments.begin(); } //!< Begin iterator
		std::vector<Attachment>::iterator end() { return m_attachments.end(); } //!< End iterator
		std::vector<Attachment>::const_iterator begin() const { return m_attachments.begin(); } //!< Begin const iterator
		std::vector<Attachment>::const_iterator end() const { return m_attachments.end(); } //!< End const iterator
	private:
		std::vector<Attachment> m_attachments;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default; //!< Destructor
		virtual void onResize(WindowResizeEvent& e) = 0; //!< On resize function
		virtual uint32_t getID() = 0; //!< Get the API specific render ID
		virtual void use() = 0; //!< Bind this framebuffer
		virtual std::shared_ptr<Texture> getTarget(uint32_t index) = 0; //!< Get the sample target at index

		static FrameBuffer* create(glm::ivec2 size, FrameBufferLayout layout); //!< API agnostic create function
		static FrameBuffer* createDefault(); //!< API agnostic create default function
	protected:
		glm::ivec2 m_size; //!< Size of the framebuffer
		FrameBufferLayout m_layout; //!< Layout of FBO attachments
		std::vector<std::shared_ptr<Texture>> m_sampledTargets;
		std::vector<std::shared_ptr<RenderBuffer>> m_nonSampledTargets;
	};
}