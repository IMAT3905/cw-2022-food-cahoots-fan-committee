/* \file renderBuffer.h */
#pragma once

#include "rendering/frameBuffer.h"
#include "events/events.h"

namespace Engine
{
	class RenderBuffer
	{
	public:
		virtual ~RenderBuffer() = default;
		virtual uint32_t getID() = 0; //!< Get API specific ID

		static RenderBuffer* create(AttachmentType type, glm::ivec2 size);
	};
}