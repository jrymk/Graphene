#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <corecrt_math_defines.h>

#include "Element.hpp"
#include "../Renderer/Structures.hpp"
#include "../Renderer/ScopedVertexArray.hpp"
#include "../Renderer/TriangleFan.hpp"
#include "../../ExceptionHandler.hpp"

namespace gue {
	/// <summary>
	/// Element bounds: Fill parent
	/// Child bounds: Same as parent
	/// Maximum children count: Unlimited
	/// </summary>
	class CircleElement : public Element {
	public:
		ScopedVertexArray* m_scopedVertexArray = nullptr;

	public:
		AVec x;
		AVec y;
		AVec radius;
		Color fillColor;
		Color backgroundColor;

	private:
		unsigned int m_pointCount = 20;

	public:
		CircleElement(const std::string& debugName) : Element(debugName) {
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		CircleElement(const std::string& debugName, AVec x, AVec y, AVec radius, Color fillColor) : Element(debugName) {
			this->x = x;
			this->y = y;
			this->radius = radius;
			this->fillColor = fillColor;
			this->backgroundColor = Color(0, 0, 0, 0);
			//this->m_pointCount = 
		}

		void build(Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";
			m_scopedVertexArray = new ScopedVertexArray();

			// build the vertex array of own
			TriangleFan backgroundRect;
			if (backgroundColor.a > 0) { // with background fill
				backgroundRect.addVertex(m_scopedVertexArray->appendVertex(position, backgroundColor));
				backgroundRect.addVertex(m_scopedVertexArray->appendVertex({ position.x + size.x, position.y }, backgroundColor));
				backgroundRect.addVertex(m_scopedVertexArray->appendVertex({ position.x + size.x, position.y + size.y }, backgroundColor));
				backgroundRect.addVertex(m_scopedVertexArray->appendVertex({ position.x, position.y + size.y }, backgroundColor));

				backgroundRect.push(m_scopedVertexArray);
			}

			TriangleFan circleShape;
			
			for (int i = 0; i < m_pointCount; i++) {
				//DBG(std::to_string(radius.evaluate(size.y)));

				circleShape.addVertex(m_scopedVertexArray->appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + radius.evaluate(min(size.x, size.y)) * cos((float)i / m_pointCount * 2 * M_PI)),
						(position.y + y.evaluate(size.y) + radius.evaluate(min(size.x, size.y)) * sin((float)i / m_pointCount * 2 * M_PI))
					),
					fillColor
				));

				circleShape.push(m_scopedVertexArray);

			}

			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build(position, size);

		}

		void push(VertexArray* vertexArray) override {
			if (m_scopedVertexArray != nullptr) {
				//push scoped data to vertex array
				m_scopedVertexArray->pushToVertexArray(vertexArray);
				
				//recursively call chilren to push
				for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
					(*child)->push(vertexArray);
	
			}
		}

	};
}