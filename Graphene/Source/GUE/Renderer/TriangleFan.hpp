#pragma once
#include <vector>
#include "VertexArray.hpp"
#include "ScopedVertexArray.hpp"

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

		void push(ScopedVertexArray* scopedVertexArray) {
			while (m_builtTriangles + 3 <= m_indices.size()) {
				m_builtTriangles++;
				scopedVertexArray->appendTriangle(m_indices[0], m_indices[m_builtTriangles], m_indices[m_builtTriangles + 1]);
			}
		}
		
		void push(VertexArray* vertexArray) {
			while (m_builtTriangles + 3 <= m_indices.size()) {
				m_builtTriangles++;
				vertexArray->appendTriangle(m_indices[0], m_indices[m_builtTriangles], m_indices[m_builtTriangles + 1]);
			}
		}
		
	};
}