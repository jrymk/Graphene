#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <corecrt_math_defines.h>

#include "Element.hpp"
#include "../Renderer/Structures.hpp"
#include "../Renderer/TriangleFan.hpp"
#include "../Batch/ScopedVertexArray.hpp"
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

		void build(Batch& batch, Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";
			ScopedVertexArray scopedVertexArray(batch);

			// build the vertex array of own
			TriangleFan backgroundRect;
			if (backgroundColor.a > 0) { // with background fill
				backgroundRect.addVertex(scopedVertexArray.appendVertex(position, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x, position.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x, position.y + size.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x, position.y + size.y }, backgroundColor));

				backgroundRect.push(&scopedVertexArray);
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

				circleShape.addVertex(scopedVertexArray.appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + radiusTemp * cos((float)i / m_pointCount * 2 * M_PI)),
						(position.y + y.evaluate(size.y) + radiusTemp * sin((float)i / m_pointCount * 2 * M_PI))
					),
					fillColor
				));
			}

			circleShape.push(&scopedVertexArray);

			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build(batch, position, size);

		}

	};
}