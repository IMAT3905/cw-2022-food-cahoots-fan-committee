/* \file indirectBuffer.h */
#pragma once

#include <cstdint>

namespace Engine
{
	struct DrawElementsIndirectCommand
	{
		uint32_t drawCount; //!< Draw count of geometry
		uint32_t instanceCount; //!< Number of instances to be drawn
		uint32_t firstIndex; //!< First index in the IBO of geometry
		uint32_t firstVertex; //!< First vertex in the IBO of geometry
		uint32_t firstInstance; //!< First instance in the IBO of geometry
	};

	class IndirectBuffer
	{
	public:
		virtual ~IndirectBuffer() = default;
		virtual inline uint32_t getID() const = 0;
		virtual inline uint32_t getCount() const = 0;
		virtual void edit(DrawElementsIndirectCommand* commands, uint32_t count, uint32_t offset) = 0;
		static IndirectBuffer* create(DrawElementsIndirectCommand* commands, uint32_t count);
	};
}