/** \file textureUnitManager.h */
#pragma once

#include <vector>

namespace Engine
{
	/** \class TextureUnitManager
	** \brief A class for a texture unit manager
	*/
	class TextureUnitManager
	{
	public:
		TextureUnitManager(uint32_t capacity) : m_capacity(capacity), m_buffer(capacity, 0xFFFFFFFF) {} //!< Constructor \param capacity is the number of textures
		inline bool full() { return m_full; } //!< Is the buffer full?
		void clear(); //!< Clear and reset the buffer
		bool getUnit(uint32_t textureID, uint32_t& textureUnit); //!< Returns whether or not the texture needs binding to the unit \param textureID is the ID of the texture that is used to check if it needs binding \param textureUnit is the unit containing the textures
	private:
		uint32_t m_capacity; //!< Capacity of ring buffer
		std::vector<uint32_t> m_buffer; //!< Internal buffer
		bool m_full = false; //!< Is the buffer full?
		uint32_t m_head = 0; //!< Head of the buffer
		uint32_t m_tail = 0; //!< Tail of the buffer
	};
}