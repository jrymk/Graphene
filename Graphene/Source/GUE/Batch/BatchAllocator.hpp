#pragma once
#include "VertexArray.hpp"
#include "Batch.hpp"

namespace gue {

	// defines the start and end of allocated vertex array space
	class BatchAllocator {
	public:
		enum class AllocateMode {
			SPECIFY,
			AUTO_GROW,
			DO_NOT_GROW,
		};

	private:
		VertexArray* m_vertexArray;

		// after a successful allocation and build this value will be set false
		// if the value is true, index buffer will be re-allocated
		bool pendingRedraw = true;

		// since without allocating vertices a repeated draw and re-allocate cycle will explode a vertex buffer 
		// without reset. to reduce code complexity, the vertices will need to be allocated first too
		unsigned int m_beginVertexAddress;
		unsigned int m_endVertexAddress;
		TriangleAddress m_beginTriangleAddress;
		TriangleAddress m_endTriangleAddress;

	public:
		AllocateMode allocateMode;
			
		// bind a vertex array to the batch allocator
		BatchAllocator(VertexArray vertexArray) {
			allocateMode = AllocateMode::AUTO_GROW;
			m_vertexArray = &vertexArray;
		}

		// starts the allocation, so that triangle insertion will have the correct indices
		void beginBatch() {

		}

	};

}