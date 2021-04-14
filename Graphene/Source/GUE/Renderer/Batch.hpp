#pragma once
#include <GLFW/glfw3.h>
#include <vector>
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

		VertexArray* getVertexArray() {
			return m_vertexArray;
		}
		
	};
}