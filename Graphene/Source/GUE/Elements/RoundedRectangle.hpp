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
	/// Maximum children count: Don't
	/// </summary>
	class RoundedRectangle : public Element {
	public:
		ScopedVertexArray* m_scopedVertexArray = nullptr;

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
		AVec w;
		AVec h;
		AVec radius;
		RadiusSizingMode radiusSizingMode;
		bool useSharedRadius;
		AVec radiusTopLeft;
		AVec radiusTopRight;
		AVec radiusBottomLeft;
		AVec radiusBottomRight;
		Color fillColor;
		Color backgroundColor;

	private:
		unsigned int m_pointCount = 40;

	public:
		RoundedRectangle(const std::string& debugName) : Element(debugName) {
			this->x = { 0.0, 0.0 };
			this->y = { 0.0, 0.0 };
			this->w = { 1.0, 0.0 };
			this->h = { 1.0, 0.0 };
			this->radius = { 0.2, 0.0 };
			this->radiusSizingMode = RadiusSizingMode::RELATIVE_TO_MIN;
			this->useSharedRadius = true;
			this->radiusTopLeft = { 0.0, 0.0 };
			this->radiusTopRight = { 0.0, 0.0 };
			this->radiusBottomLeft = { 0.0, 0.0 };
			this->radiusBottomRight = { 0.0, 0.0 };
			this->fillColor = Color(255, 0, 0, 50);
			this->backgroundColor = Color(0, 0, 0, 0);

		}

		RoundedRectangle(const std::string& debugName, AVec x, AVec y, AVec w, AVec h, AVec radius, Color fillColor) : Element(debugName) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
			this->radius = radius;
			this->radiusSizingMode = RadiusSizingMode::RELATIVE_TO_MIN;
			this->useSharedRadius = true;
			this->radiusTopLeft = { 0.0, 0.0 };
			this->radiusTopRight = { 0.0, 0.0 };
			this->radiusBottomLeft = { 0.0, 0.0 };
			this->radiusBottomRight = { 0.0, 0.0 };
			this->fillColor = fillColor;
			this->backgroundColor = Color(0, 0, 0, 0);

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

			TriangleFan roundedRectangleShape;

			float radiusTemp = useSharedRadius ? radius.evaluate(radiusSizingRelation) : radiusBottomRight.evaluate(radiusSizingRelation);
			
			for (unsigned int i = 0; i <= m_pointCount / 4; i++) {
				roundedRectangleShape.addVertex(m_scopedVertexArray->appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + w.evaluate(size.x) + radiusTemp * cos((float)i / m_pointCount * 2 * M_PI) - 1),
						(position.y + y.evaluate(size.y) + h.evaluate(size.y) + radiusTemp * sin((float)i / m_pointCount * 2 * M_PI) - 1)
					),
					fillColor
				));
			}

			radiusTemp = useSharedRadius ? radius.evaluate(radiusSizingRelation) : radiusBottomLeft.evaluate(radiusSizingRelation);

			for (unsigned int i = m_pointCount / 4; i <= m_pointCount / 2; i++) {
				roundedRectangleShape.addVertex(m_scopedVertexArray->appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + radiusTemp * cos((float)i / m_pointCount * 2 * M_PI) + 1),
						(position.y + y.evaluate(size.y) + h.evaluate(size.y) + radiusTemp * sin((float)i / m_pointCount * 2 * M_PI) - 1)
					),
					fillColor
				));
			}

			radiusTemp = useSharedRadius ? radius.evaluate(radiusSizingRelation) : radiusTopLeft.evaluate(radiusSizingRelation);

			for (unsigned int i = m_pointCount / 2; i <= 3 * m_pointCount / 4; i++) {
				roundedRectangleShape.addVertex(m_scopedVertexArray->appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + radiusTemp * cos((float)i / m_pointCount * 2 * M_PI) + 1),
						(position.y + y.evaluate(size.y) + radiusTemp * sin((float)i / m_pointCount * 2 * M_PI) + 1)
					),
					fillColor
				));
			}

			radiusTemp = useSharedRadius ? radius.evaluate(radiusSizingRelation) : radiusTopRight.evaluate(radiusSizingRelation);

			for (unsigned int i = 3 * m_pointCount / 4; i <= m_pointCount; i++) {
				roundedRectangleShape.addVertex(m_scopedVertexArray->appendVertex(
					Vec2f((position.x + x.evaluate(size.x) + w.evaluate(size.x) + radiusTemp * cos((float)i / m_pointCount * 2 * M_PI) - 1),
						(position.y + y.evaluate(size.y) + radiusTemp * sin((float)i / m_pointCount * 2 * M_PI) + 1)
					),
					fillColor
				));
			}

			
			roundedRectangleShape.push(m_scopedVertexArray);

			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build(position, size);

		}

		void push(VertexArray* vertexArray) override {
			if (m_scopedVertexArray != nullptr) {
				//push scoped data to vertex array
				m_scopedVertexArray->pushToVertexArray(vertexArray);
				m_scopedVertexArray->deleteScope();

				//recursively call chilren to push
				for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
					(*child)->push(vertexArray);

			}
		}

	};
}