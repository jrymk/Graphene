#pragma once
#include "VertexArray.hpp"
#include "BatchAllocator.hpp"

namespace gue {

	// defines the start and end of allocated vertex array space
	class BatchAllocator {
	public:
		enum class AllocateMode {
			DO_NOT_GROW,     // don't accept any insertion after vertex array is full
			GROW_TO_FIT,     // grow whatever amount of space needed to fit the newly inserted elements
			GROW_TO_DOUBLE,  // doubles the allocation size if full just like how a std::vector work (default)
		};

	private:
		VertexArray* m_vertexArray;

	public:
		// minimum allocation size, so time-consuming reallocation and redraw can be reduced
		unsigned int vertexMinSize = 0;
		unsigned int triangleMinSize = 0;

		// true when a redraw for elements afterwards is needed
		// happens when overflowed
		bool pendingRedraw = true;

	private:
		// since without allocating vertices a repeated draw and re-allocate cycle will explode a vertex buffer 
		// without reset. to reduce code complexity, the vertices will need to be allocated first too
		// note: close begin open end
		unsigned int m_beginVertexAddress;
		unsigned int m_endVertexAddress;
		TriangleAddress m_beginTriangleAddress;
		TriangleAddress m_endTriangleAddress;

	public:
		AllocateMode allocateMode;

		// bind a vertex array to the batch allocator
		BatchAllocator(VertexArray& vertexArray) {
			allocateMode = AllocateMode::GROW_TO_DOUBLE;
			m_vertexArray = &vertexArray;

			// set to 0 so size is undetermined and size is dependent to added items
			// after these values are set, overflow events will be handled ungracefully
			m_endVertexAddress = 0;
			m_endTriangleAddress = { m_vertexArray, 0 };
		}

		// run this when redrawing an element to allocate a chunk of memory
		void beginAlloc() {
			m_beginVertexAddress = m_vertexArray->nextAvailableVertex;
			m_beginTriangleAddress = { m_vertexArray, m_vertexArray->nextAvailableTriangle };
		}

		// use these append method so overflow is safely handled
		unsigned int add(Vec2f position, Color color) {
			if (m_endVertexAddress != 0 && m_endVertexAddress <= m_vertexArray->nextAvailableVertex)
				m_endVertexAddress = m_beginVertexAddress + (m_endVertexAddress - m_beginVertexAddress) * 2;

			m_vertexArray->nextAvailableVertex++;
			return m_vertexArray->appendVertex(position, color) - m_beginVertexAddress;
		}

		TriangleAddress add(unsigned int vertex0, unsigned int vertex1, unsigned int vertex2) {
			if (m_endTriangleAddress.id != 0 && m_endTriangleAddress.id <= m_vertexArray->nextAvailableTriangle)
				m_endTriangleAddress.id = m_beginTriangleAddress.id + (m_endTriangleAddress.id - m_beginTriangleAddress.id) * 2;

			m_vertexArray->nextAvailableVertex++;
			return { m_vertexArray, m_vertexArray->appendTriangle(vertex0, vertex1, vertex2) - m_beginTriangleAddress.id };
		}


		// run this after you are done
		void endAlloc() {
			m_endVertexAddress = max(m_vertexArray->getVerticesCount(), m_beginVertexAddress + vertexMinSize);
			m_endTriangleAddress.id = max(m_vertexArray->getTriangleCount(), m_beginTriangleAddress.id + triangleMinSize);

			m_vertexArray->nextAvailableVertex = m_endVertexAddress;
			m_vertexArray->nextAvailableTriangle = m_endTriangleAddress.id;
		}

		// clear the triangles but keep the allocated space
		void clearAll() {
			for (int i = m_beginTriangleAddress.id; i < m_beginTriangleAddress.id; i++) {
				(*m_vertexArray->getIndices())[i * 3] = 0;
				(*m_vertexArray->getIndices())[i * 3 + 1] = 0;
				(*m_vertexArray->getIndices())[i * 3 + 2] = 0;
			}
		}

		// does not clear the written data but does free up space for future vertices and triangles adding
		void unallocateBatch() {
			m_vertexArray->nextAvailableVertex = m_beginVertexAddress;
			m_vertexArray->nextAvailableTriangle = m_beginTriangleAddress.id;

			m_endVertexAddress = 0;
			m_endTriangleAddress = { m_vertexArray, 0 };
		}

	};

}