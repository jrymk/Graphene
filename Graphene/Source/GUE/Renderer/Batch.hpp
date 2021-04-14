#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include "Vertex.hpp"
#include "VertexArray.hpp"
#include "Window.hpp"

namespace gue {
	class Batch {
	private:
		VertexArray* m_vertexArray;
		Shader* m_shader;

	public:
		Batch(Window& window, Shader& shader) {
			m_vertexArray = new VertexArray(&window);
			m_shader = &shader;
		}

		void clearBatch() {
			m_vertexArray->clear();
		}

		void drawBatch() {
			m_vertexArray->draw(*m_shader);
		}

		unsigned int getVerticesCount() {
			return m_vertexArray->getVertices()->size();
		}
		
		unsigned int getTriangleCount() {
			return m_vertexArray->getIndices()->size() / 3;
		}

		unsigned int getIndicesCount() {
			return m_vertexArray->getIndices()->size();
		}

		VertexArray* getVertexArray() {
			return m_vertexArray;
		}
		
	};
}