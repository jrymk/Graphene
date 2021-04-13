#pragma once
#include <vector>
#include "Structures.hpp"
#include "VertexArray.hpp"

namespace gue {
	class ScopedVertexArray {
	private:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

	public:
		ScopedVertexArray() {
			
		}

		std::vector<Vertex>* getVertices() {
			return &m_vertices;
		}

		std::vector<unsigned int>* getIndices() {
			return &m_indices;
		}

		unsigned int appendVertex(Vec2f pixelCoord, Color color255) {
			m_vertices.emplace_back(pixelCoord, color255.toColorf());
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

		void pushToVertexArray(VertexArray* vertexArray) {
			unsigned int startingIndex = vertexArray->getVertices()->size();

			m_vertices.emplace_back();

			
			for (int i = 0; i < m_vertices.size(); i++)
				vertexArray->getVertices()->emplace_back(Vertex(
					Vec2f(m_vertices[i].position.x * 2.0f / vertexArray->getWindow()->getFramebufferSize().toFloat().x - 1.0f,
						-m_vertices[i].position.y * 2.0f / vertexArray->getWindow()->getFramebufferSize().toFloat().y + 1.0f),
					m_vertices[i].color
				));

			for (int i = 0; i < m_indices.size(); i++)
				vertexArray->getIndices()->emplace_back(m_indices[i] + startingIndex);

		}

		void deleteScope() {
			delete this;
		}
		
	};
}