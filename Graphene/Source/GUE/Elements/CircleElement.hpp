#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <corecrt_math_defines.h>

#include "Element.hpp"
#include "../Renderer/Structures.hpp"
#include "../Renderer/VertexArray.hpp"
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
				//DBG(std::to_string(radius.evaluate(size.y)));
				vertexArray->appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + (radius.evaluate(size.x) * cos((float)i / m_pointCount * 2 * M_PI))),
						(position.y + y.evaluate(size.y) + (radius.evaluate(size.x) * sin((float)i / m_pointCount * 2 * M_PI)))
					),
					color
				);

				if (i >= 2) {
					vertexArray->appendIndex(0);
					vertexArray->appendIndex(i - 1);
					vertexArray->appendIndex(i);
				}
			}
		}

	};
}