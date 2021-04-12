#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "Shaders.hpp"
#include "Vertex.hpp"
#include "Window.hpp"
#include "../../ExceptionHandler.hpp"

namespace gue {
	class VertexArray {
	private:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		Window* m_window;
		
	public:
		VertexArray(Window& window) {
			m_window = &window;
		}
		
		void bindWindow(Window& window) {
			m_window = &window;
		}

		std::vector<Vertex>* getVertices() {
			return &m_vertices;
		}

		std::vector<unsigned int>* getIndices() {
			return &m_indices;
		}

		void appendVertex(Vec2f pixelCoord, Color color255) {
			m_vertices.push_back(Vertex(
				Vec2f( pixelCoord.x * 2.0f / m_window->getFramebufferSize().toFloat().x - 1.0f, pixelCoord.y * 2.0f / m_window->getFramebufferSize().toFloat().y - 1.0f ),
				color255.toColorf()
			));
		}

		void appendIndex(unsigned int index) {
			m_indices.push_back(index);
			
		}
		
		void clear() {
			m_vertices.clear();
			m_indices.clear();
		}

		void printContents() {
			DBG("Vertices: ");
			for (int i = 0; i < m_vertices.size(); i++)
				DBG(
					std::to_string(m_vertices[i].position.x) + std::string(", ") +
					std::to_string(m_vertices[i].position.y) + std::string(", ") +
					std::to_string(m_vertices[i].color.r) + std::string(", ") +
					std::to_string(m_vertices[i].color.g) + std::string(", ") +
					std::to_string(m_vertices[i].color.b) + std::string(", ") +
					std::to_string(m_vertices[i].color.a)
				);
			DBG("Indices: ");
			for (int i = 0; i < m_indices.size(); i += 3)
				DBG(
					std::to_string(m_indices[i]) + std::string(", ") +
					std::to_string(m_indices[i + 1]) + std::string(", ") +
					std::to_string(m_indices[i + 2])
				);
		}

		void draw(Shader& shader) {
			GLuint VAO; // vertex array object
			GLuint VBO; // vertex buffer object
			GLuint EBO; // index buffer object

			GLCall(glGenVertexArrays(1, &VAO));
			GLCall(glGenBuffers(1, &VBO));
			GLCall(glGenBuffers(1, &EBO));

			GLCall(glBindVertexArray(VAO));

			GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
			GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW));

			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW));

			// vertex positions
			GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));
			GLCall(glEnableVertexAttribArray(0));

			// vertex colors
			GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color)));
			GLCall(glEnableVertexAttribArray(1));

			shader.useShader();
			
			GLCall(glBindVertexArray(VAO));
			GLCall(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));

			GLCall(glDeleteVertexArrays(1, &VAO));
			GLCall(glDeleteBuffers(1, &VBO));
			GLCall(glDeleteBuffers(1, &EBO));
			
		}
		
	};
}