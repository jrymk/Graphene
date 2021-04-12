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
	/// </summary>
	class CircleElement : public Element {
	private:
		
	public:
		AVec x;
		AVec y;
		AVec radius;
		Color color;

	private:
		unsigned int m_pointCount = 40;

	public:
		CircleElement() : Element() {

		}

		CircleElement(AVec x, AVec y, AVec radius, Color color) : Element() {
			this->x = x;
			this->y = y;
			this->radius = radius;
			this->color = color;
		}

		void generateBuffer(VertexArray* vertexArray, Vec2f position, Vec2f size) {
			ScopedVertexArray scopedVertexArray(vertexArray);
			
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
		}

	};
}