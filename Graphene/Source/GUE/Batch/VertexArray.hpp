#pragma once

#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.hpp"
#include "../Renderer/Window.hpp"
#include "../Renderer/Shaders.hpp"
#include "../../ExceptionHandler.hpp"

namespace gue {

	class VertexArray {

	public:

		// one vertex array have one transformation, useful for fast hardware accelerated scroll and zoom, pan, rotation
		glm::mat4 transformation = glm::mat2(1.0f);

		unsigned int nextAvailableVertex = 0;
		unsigned int nextAvailableTriangle = 0;

	private:

		// this class only contains the basics
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		// buffer ids
		GLuint m_VAO = 0; // vertex array object
		GLuint m_VBO = 0; // vertex buffer object
		GLuint m_EBO = 0; // element buffer object

		Window* m_window;

	public:

		VertexArray(Window& window) {
			// a vertex array is tied to a window object for the screen resolution data and such
			m_window = &window;
		}

		void bindWindow(Window& window) {
			m_window = &window;
		}

		// returns the raw vertices buffer data
		std::vector<Vertex>* getVertices() {
			return &m_vertices;
		}

		// returns the raw indices buffer data
		std::vector<unsigned int>* getIndices() {
			return &m_indices;
		}

		Window* getWindow() {
			return m_window;
		}

		// append a vertex to the end of the vertex buffer, with screen resolution translation, 
		// and returns the index of the vertex inserted
		unsigned int appendVertex(Vec2f pixelCoord, Color color255) {
			m_vertices.emplace_back(Vertex(
				Vec2f(pixelCoord.x * 2.0f / m_window->getFramebufferSize().toFloat().x - 1.0f,
					-pixelCoord.y * 2.0f / m_window->getFramebufferSize().toFloat().y + 1.0f),
				color255.toColorf()
			));
			return m_vertices.size() - 1;
		}

		// append a triangle to the end of the index buffer, and returns the id of the triangle inserted
		unsigned int appendVertex(Vec2f pixelCoord, Colorf colorF) {
			m_vertices.emplace_back(Vertex(
				Vec2f(pixelCoord.x * 2.0f / m_window->getFramebufferSize().toFloat().x - 1.0f,
					-pixelCoord.y * 2.0f / m_window->getFramebufferSize().toFloat().y + 1.0f),
				colorF
			));
			return m_vertices.size() / 3 - 1;
		}

		unsigned int getVerticesCount() {
			return m_vertices.size();
		}

		unsigned int getTriangleCount() {
			return m_indices.size() / 3 - 1;
		}

		// since the primitives are always triangles in this case, the append function is heavily tied to triangles
		// don't append a single index or else something bad may happen
		unsigned int appendTriangle(unsigned int index0, unsigned int index1, unsigned int index2) {
			m_indices.emplace_back(index0);
			m_indices.emplace_back(index1);
			m_indices.emplace_back(index2);
			return m_indices.size() / 3 - 1;
		}

		// clear function, for no particular reason
		void clear() {
			m_vertices.clear();
			m_indices.clear();
		}


		// the main draw function
		void draw(Shader& shader) {

			GLCall(glGenVertexArrays(1, &m_VAO));
			GLCall(glGenBuffers(1, &m_VBO));
			GLCall(glGenBuffers(1, &m_EBO));

			GLCall(glBindVertexArray(m_VAO));

			// allocate memory for VBOs and EBOs
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

			// apply transformation
			glUniformMatrix4fv(shader.transformationLocation, 1, GL_FALSE, glm::value_ptr(transformation));

			GLCall(glBindVertexArray(m_VAO));
			GLCall(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0));

			GLCall(glDeleteVertexArrays(1, &m_VAO));
			GLCall(glDeleteBuffers(1, &m_VBO));
			GLCall(glDeleteBuffers(1, &m_EBO));

		}


		// prints all the buffer contents for debugging
		void printContents() {
			DBG("Vertices: ");
			for (int i = 0; i < m_vertices.size(); i++) {
				DBG(
					std::to_string(m_vertices[i].position.x) + std::string(", ") +
					std::to_string(m_vertices[i].position.y) + std::string(", ") +
					std::to_string(m_vertices[i].color.r) + std::string(", ") +
					std::to_string(m_vertices[i].color.g) + std::string(", ") +
					std::to_string(m_vertices[i].color.b) + std::string(", ") +
					std::to_string(m_vertices[i].color.a)
				);
			}

			DBG("Indices: ");
			for (int i = 0; i < m_indices.size(); i += 3) {
				DBG(
					std::to_string(m_indices[i]) + std::string(", ") +
					std::to_string(m_indices[i + 1]) + std::string(", ") +
					std::to_string(m_indices[i + 2])
				);
			}

		}

	};

	// representation of a particular vertex array + a particular triangle
	struct TriangleAddress {
		VertexArray* vertexArray;
		unsigned int id;

		TriangleAddress() = default;

		TriangleAddress(VertexArray* vertexArray, unsigned int id) {
			this->vertexArray = vertexArray;
			this->id = id;
		}
	};

}