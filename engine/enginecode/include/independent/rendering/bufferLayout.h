/** \file bufferLayout.h */
#pragma once

#include "shaderDataType.h"
#include <vector>

namespace Engine
{
	/** \class VertexBufferElement
	** \brief A class which holds data about a single element in a vertex buffer layout
	*/
	class VertexBufferElement
	{
	public:
		ShaderDataType m_dataType; //!< The type of data
		uint32_t m_size; //!< The size of the element
		uint32_t m_offset; //!< The offset of the element
		bool m_normalised; //!< If the data is normalised

		VertexBufferElement() {} //!< Default constructor
		VertexBufferElement(ShaderDataType dataType, bool normalised = false) : m_dataType(dataType), m_size(SDT::size(dataType)), m_offset(0), m_normalised(normalised) {} //!< Constructor for vertex buffer element \param datatype is the type of data \param normalised is if the data is normalised
	};

	/** \class UniformBufferElement
	** \brief A class which holds data about a single element in a uniform buffer layout
	*/
	class UniformBufferElement
	{
	public:
		const char* m_name; //!< The name of the data
		ShaderDataType m_dataType; //!< The type of data
		uint32_t m_size; //!< The size of the element
		uint32_t m_offset; //!< The offset of the element

		UniformBufferElement() {} //!< Default constructor
		UniformBufferElement(const char * name, ShaderDataType dataType) : m_name(name), m_dataType(dataType), m_size(SDT::std140alignment(dataType)), m_offset(0) {} //!< Constructor for uniform buffer element \param name is the name of the data \param datatype is the type of data
	};

	/** \class BufferLayout
	** \brief Abstraction of the notion of a buffer layout
	*/
	template <class G>
	class BufferLayout
	{
	public:
		BufferLayout<G>() {}; //!< Default constructor
		BufferLayout<G>(const std::initializer_list<G>& element, uint32_t stride = 0) : m_elements(element), m_stride(stride) { calcStrideAndOffset(); } //!< Constructor for a buffer layout \param element is the data in the buffer layout \param stride is the size of the data of bytes in a buffer line
		inline uint32_t getStride() const { return m_stride; } //!< Returns the stride of the data
		void addElement(G element); //!< Function to add an element to the layout \param element is the data to be added
		inline typename std::vector<G>::iterator begin() { return m_elements.begin(); } //!< Returns the first piece of data from the elements which can be changed
		inline typename std::vector<G>::iterator end() { return m_elements.end(); } //!< Returns the last piece of data from the elements which can be changed
		inline typename std::vector<G>::const_iterator begin() const { return m_elements.begin(); } //!< Returns the first piece of data from the elements which cannot be changed
		inline typename std::vector<G>::const_iterator end() const { return m_elements.end(); } //!< Returns the last piece of data from the elements which cannot be changed
	private:
		std::vector<G> m_elements; //!< Buffer elements
		uint32_t m_stride; //!< Width is bytes of a buffer line
		void calcStrideAndOffset(); //!< Calculate stride and offsets based on elements
	};

	template <class G>
	void BufferLayout<G>::addElement(G element)
	{
		m_elements.push_back(element);
		calcStrideAndOffset();
	}

	template <class G>
	void BufferLayout<G>::calcStrideAndOffset()
	{
		uint32_t l_offset = 0;

		for (auto& element : m_elements)
		{
			element.m_offset = l_offset;
			l_offset += element.m_size;
		}

		if (m_stride == 0) m_stride = l_offset;
	}

	using VertexBufferLayout = BufferLayout<VertexBufferElement>;
	using UniformBufferLayout = BufferLayout<UniformBufferElement>;
}