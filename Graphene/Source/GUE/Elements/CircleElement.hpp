#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <corecrt_math_defines.h>

#include "Element.hpp"
#include "../Renderer/Structures.hpp"
#include "../Renderer/ScopedVertexArray.hpp"
#include "../../ExceptionHandler.hpp"

namespace gue {
	/// <summary>
	/// Element bounds: Fill parent
	/// Child bounds: Same as parent
	/// Maximum children count: Unlimited
	/// </summary>
	class CircleElement : public Element {
	public:

	public:
		AVec x;
		AVec y;
		AVec radius;
		Color color;

	private:
		unsigned int m_pointCount = 20;

	public:
		CircleElement(const std::string& debugName) : Element(debugName) {

		}

		CircleElement(const std::string& debugName, AVec x, AVec y, AVec radius, Color color) : Element(debugName) {
			this->x = x;
			this->y = y;
			this->radius = radius;
			this->color = color;
			//this->m_pointCount = 
		}

		void build(VertexArray* vertexArray, Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";
			ScopedVertexArray scopedVertexArray(vertexArray);

			// build the vertex array of own
			for (int i = 0; i < m_pointCount; i++) {
				//DBG(std::to_string(radius.evaluate(size.y)));
				scopedVertexArray.appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + radius.evaluate(min(size.x, size.y)) * cos((float)i / m_pointCount * 2 * M_PI)),
						(position.y + y.evaluate(size.y) + radius.evaluate(min(size.x, size.y)) * sin((float)i / m_pointCount * 2 * M_PI))
					),
					color
				);

				if (i >= 2) {
					scopedVertexArray.appendIndex(i);
					scopedVertexArray.appendIndex(i - 1);
					scopedVertexArray.appendIndex(0);
				}
			}

			//recursively call chilren
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++) {
				//std::cout << (*child)->debugName << "\n";
				(*child)->build(vertexArray, position, size);
			}

		}

	};
}