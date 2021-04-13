#pragma once
#include "VertexArray.hpp"

namespace gue {
	class ScopedVertexArray {
	private:
		VertexArray* m_vertexArray;
		unsigned long long m_startingIndex;

	public:
		ScopedVertexArray(VertexArray* vertexArray) {
			m_vertexArray = vertexArray;
		}

		void allocate() {
			m_startingIndex = m_vertexArray->getVertices()->size();
			
		}

		VertexArray* getVertexArray() {
			return m_vertexArray;
		}
		
		unsigned long long appendVertex(Vec2f pixelCoord, Color color255) {
			m_vertexArray->getVertices()->emplace_back(Vertex(
				Vec2f(pixelCoord.x * 2.0f / m_vertexArray->getWindow()->getFramebufferSize().toFloat().x - 1.0f,
					pixelCoord.y * 2.0f / m_vertexArray->getWindow()->getFramebufferSize().toFloat().y - 1.0f),
				color255.toColorf()
			));
			return m_vertexArray->getVertices()->size() - 1 - m_startingIndex;
		}

		void appendIndex(unsigned int index) {
			m_vertexArray->getIndices()->emplace_back(index + m_startingIndex);

		}
		
	};
}