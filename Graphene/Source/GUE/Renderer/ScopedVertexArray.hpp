#pragma once
#include <vector>
#include "Structures.hpp"
#include "Batch.hpp"
#include "VertexArray.hpp"

namespace gue {
	class ScopedVertexArray {
	private:
		VertexArray* m_vertexArray;
		unsigned long long m_startingIndex;
		unsigned long long m_startingIndicesSize;

	public:
		ScopedVertexArray(Batch& batch) {
			this->m_vertexArray = batch.getVertexArray();
			this->m_startingIndex = batch.getVertexArray()->getVertices()->size();
			this->m_startingIndicesSize = batch.getVertexArray()->getIndices()->size();
		}

		std::vector<Vertex>* getVertices() {
			return m_vertexArray->getVertices();
		}

		std::vector<unsigned int>* getIndices() {
			return m_vertexArray->getIndices();
		}

		unsigned int appendVertex(Vec2f pixelCoord, Color color255) {
			m_vertexArray->appendVertex(pixelCoord, color255);
			return m_vertexArray->getVertices()->size() - 1 - m_startingIndex;
		}

		void appendIndex(unsigned int index) {
			m_vertexArray->appendIndex(m_startingIndex + index);
		}

		void appendIndex(unsigned int index0, unsigned int index1, unsigned int index2) {
			m_vertexArray->appendIndex(m_startingIndex + index0);
			m_vertexArray->appendIndex(m_startingIndex + index1);
			m_vertexArray->appendIndex(m_startingIndex + index2);
		}

		void restore() {
			m_vertexArray->getVertices()->resize(m_startingIndex);
			m_vertexArray->getIndices()->resize(m_startingIndicesSize);
		}

		void printContents() {
			DBG("Vertices: ");
			for (int i = 0; i < m_vertexArray->getVertices()->size(); i++)
				DBG(
					std::to_string((*m_vertexArray->getVertices())[i].position.x) + std::string(", ") +
					std::to_string((*m_vertexArray->getVertices())[i].position.y) + std::string(", ") +
					std::to_string((*m_vertexArray->getVertices())[i].color.r) + std::string(", ") +
					std::to_string((*m_vertexArray->getVertices())[i].color.g) + std::string(", ") +
					std::to_string((*m_vertexArray->getVertices())[i].color.b) + std::string(", ") +
					std::to_string((*m_vertexArray->getVertices())[i].color.a)
				);
			DBG("Indices: ");
			for (int i = 0; i < m_vertexArray->getIndices()->size(); i += 3)
				DBG(
					std::to_string((*m_vertexArray->getIndices())[i]) + std::string(", ") +
					std::to_string((*m_vertexArray->getIndices())[i + 1]) + std::string(", ") +
					std::to_string((*m_vertexArray->getIndices())[i + 2])
				);
		}

	};
}