#pragma once
#include <vector>
#include "../Batch/VertexArray.hpp"

namespace gue {
	class TriangleFan {

	private:
		std::vector<unsigned int> m_indices;
		unsigned int m_builtTriangles = 0;

	public:
		TriangleFan() = default;

		void addVertex(unsigned int index) {
			m_indices.emplace_back(index);
		}

		void push(BatchAllocator* batch) {
			while (m_builtTriangles + 3 <= m_indices.size()) {
				m_builtTriangles++;
				batch->add(m_indices[0], m_indices[m_builtTriangles], m_indices[m_builtTriangles + 1]);
			}
		}
		
	};
}