#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <corecrt_math_defines.h>

#include "Element.hpp"
#include "../Renderer/Structures.hpp"
#include "../Renderer/TriangleFan.hpp"
#include "../../ExceptionHandler.hpp"

namespace gue {
	/// <summary>
	/// Element bounds: Fill parent
	/// Child bounds: Same as parent
	/// Maximum children count: Don't
	/// </summary>
	class CircleShape : public Element {
	public:

		enum class RadiusSizingMode {
			NONE,
			RELATIVE_TO_MIN,
			RELATIVE_TO_MAX,
			RELATIVE_TO_WIDTH,
			RELATIVE_TO_HEIGHT,
			RELATIVE_TO_AVERAGE,

		};

	public:
		AVec x;
		AVec y;
		AVec radius;
		RadiusSizingMode radiusSizingMode;
		Color fillColor;
		Color backgroundColor;

	private:
		unsigned int m_pointCount = 40;

	public:
		CircleShape(const std::string& debugName) : Element(debugName) {
			this->x = AVec(0.5, 0.0);
			this->y = AVec(0.5, 0.0);
			this->radius = AVec(0.5, 0.0);
			this->radiusSizingMode = RadiusSizingMode::RELATIVE_TO_MIN;
			this->fillColor = Color(255, 0, 0, 50);
			this->backgroundColor = Color(0, 0, 0, 0);

		}

		CircleShape(const std::string& debugName, AVec x, AVec y, AVec radius, Color fillColor) : Element(debugName) {
			this->x = x;
			this->y = y;
			this->radius = radius;
			this->radiusSizingMode = RadiusSizingMode::RELATIVE_TO_MIN;
			this->fillColor = fillColor;
			this->backgroundColor = Color(0, 0, 0, 0);
			//this->m_pointCount = 
		}

		void build(VertexArray& vertexArray, Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";
			BatchAllocator batch(vertexArray);
			batch.beginAlloc();
			// build the vertex array of own
			TriangleFan backgroundRect;
			if (backgroundColor.a > 0) { // with background fill
				backgroundRect.addVertex(batch.add(position, backgroundColor));
				backgroundRect.addVertex(batch.add({ position.x + size.x, position.y }, backgroundColor));
				backgroundRect.addVertex(batch.add({ position.x + size.x, position.y + size.y }, backgroundColor));
				backgroundRect.addVertex(batch.add({ position.x, position.y + size.y }, backgroundColor));

				backgroundRect.push(&batch);
			}


			float radiusSizingRelation = 0;
			switch (radiusSizingMode) {
			case RadiusSizingMode::NONE:
				radiusSizingRelation = 0;
				break;
			case RadiusSizingMode::RELATIVE_TO_MIN:
				radiusSizingRelation = min(size.x, size.y);
				break;
			case RadiusSizingMode::RELATIVE_TO_MAX:
				radiusSizingRelation = max(size.x, size.y);
				break;
			case RadiusSizingMode::RELATIVE_TO_WIDTH:
				radiusSizingRelation = size.x;
				break;
			case RadiusSizingMode::RELATIVE_TO_HEIGHT:
				radiusSizingRelation = size.y;
				break;
			case RadiusSizingMode::RELATIVE_TO_AVERAGE:
				radiusSizingRelation = (size.x + size.y) / 2.0;
				break;
			}

			TriangleFan circleShape;

			float radiusTemp = radius.evaluate(radiusSizingRelation);

			for (unsigned int i = 0; i < m_pointCount; i++) {
				//DBG(std::to_string(radius.evaluate(size.y)));

				circleShape.addVertex(batch.add(
					Vec2f((position.x + x.evaluate(size.x) + radiusTemp * cos((float)i / m_pointCount * 2 * M_PI)),
						(position.y + y.evaluate(size.y) + radiusTemp * sin((float)i / m_pointCount * 2 * M_PI))
					),
					fillColor
				));
			}

			circleShape.push(&batch);

			//recursively call children to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build(vertexArray, position, size);

			batch.endAlloc();
		}

	};
}