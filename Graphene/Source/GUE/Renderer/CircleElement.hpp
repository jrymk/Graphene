#pragma once
#define _USE_MATH_DEFINES

#include <math.h>
#include "Element.hpp"
#include "Structures.hpp"
#include "VertexArray.hpp"
#include "../../ExceptionHandler.hpp"

namespace gue {
	class CircleElement : public Element {
	public:
		AVec x;
		AVec y;
		AVec radius;
		Color color;

	private:
		unsigned int m_pointCount = 40;

	public:
		void generateBuffer(VertexArray* vertexArray, Vec2f position, Vec2f size) {
			for (int i = 0; i < m_pointCount; i++) {
				vertexArray->appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + radius.evaluate(min(size.x, size.y)) * cos((float)i / m_pointCount * 2 * M_PI)),
						(position.y + y.evaluate(size.y) + radius.evaluate(min(size.x, size.y)) * sin((float)i / m_pointCount * 2 * M_PI))
					),
					color
				);
				
				if (i >= 2) {
					vertexArray->getIndices()->push_back(0);
					vertexArray->getIndices()->push_back(i - 1);
					vertexArray->getIndices()->push_back(i);
				}
			}
		}

	};
}