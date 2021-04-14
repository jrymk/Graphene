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

		GLuint m_VAO; // vertex array object
		GLuint m_VBO; // vertex buffer object
		GLuint m_EBO; // index buffer object


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

		Window* getWindow() {
			return m_window;
		}
		
		unsigned int appendVertex(Vec2f pixelCoord, Color color255) {
			m_vertices.emplace_back(Vertex(
				Vec2f(pixelCoord.x * 2.0f / m_window->getFramebufferSize().toFloat().x - 1.0f, 
					-pixelCoord.y * 2.0f / m_window->getFramebufferSize().toFloat().y + 1.0f),
				color255.toColorf()
			));
			return m_vertices.size() - 1;
		}

		void appendIndex(unsigned int index) {
			m_indices.emplace_back(index);

		}

		void appendIndex(unsigned int index0, unsigned int index1, unsigned int index2) {
			m_indices.emplace_back(index0);
			m_indices.emplace_back(index1);
			m_indices.emplace_back(index2);

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
			
			GLCall(glGenVertexArrays(1, &m_VAO));
			GLCall(glGenBuffers(1, &m_VBO));
			GLCall(glGenBuffers(1, &m_EBO));

			GLCall(glBindVertexArray(m_VAO));

			// allocate memory for vbos and ebos
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
			GLCall(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW));

			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_DYNAMIC_DRAW));

			// set up vertex attributes
			GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));
			GLCall(glEnableVertexAttribArray(0));

			GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color)));
			GLCall(glEnableVertexAttribArray(1));

			// use specified shader
			shader.useShader();
			
			GLCall(glBindVertexArray(m_VAO));
			GLCall(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));

			GLCall(glDeleteVertexArrays(1, &m_VAO));
			GLCall(glDeleteBuffers(1, &m_VBO));
			GLCall(glDeleteBuffers(1, &m_EBO));

		}

	};
}