#pragma once
#include <vector>
#include "../Renderer/Vertex.hpp"
#include "../Renderer/Structures.hpp"
#include "../Renderer/VertexArray.hpp"
#include "../../ExceptionHandler.hpp"

namespace gue {
	class Element {
	public:
		std::string debugName;
		
	protected:
		Element* m_parentElement;
		std::vector<Element*> m_childrenElements;
	
	public:
		virtual void build(VertexArray* vertexArray, Vec2f position, Vec2f size) {
			
		}

		Element(const std::string& debugName) {
			this->debugName = debugName;
			m_parentElement = nullptr;
		}

		void add(Element* child, long long slot) {
			if (child != nullptr) { // parent element defined
				if (slot < 0) // slot not defined, for unordered containers
					m_childrenElements.emplace_back(child);
				else {
					if (m_childrenElements.size() < slot + 1)
						m_childrenElements.resize(slot + 1);
					m_childrenElements[slot] = child;
				}
			}

			//for (int i = 0; i < m_childrenElements.size(); i++)
			//	std::cout << m_childrenElements[i]->debugName << "\n";

			child->linkParentElement(this);
		}
		
		void linkParentElement(Element* parentElement) {
			if (m_parentElement != nullptr) { // currently under a parent element
				m_parentElement->m_childrenElements.erase(std::find(m_childrenElements.begin(), m_childrenElements.end(), this));
			}
			
			m_parentElement = parentElement;
		}

		void deleteElement() {
			for (int i = 0; i < m_childrenElements.size(); i++) {
				m_childrenElements[i]->deleteElement();
			}
			
			delete this;
		}
		
		void deleteChildren() {
			for (int i = 0; i < m_childrenElements.size(); i++) {
				m_childrenElements[i]->deleteElement();
			}

		}
		
		
	public:


		
		
	};
	
}
