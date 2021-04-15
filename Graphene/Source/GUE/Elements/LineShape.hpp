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
	/// Child bounds: Fill parent
	/// Maximum children count: Don't
	/// </summary>
	class LineShape : public Element {

	public:
		AVec pointAx;
		AVec pointAy;
		AVec pointBx;
		AVec pointBy;
		AVec thickness;
		Color fillColor;
		Color backgroundColor;
		AVec offset;

	private:

	public:
		LineShape(const std::string& debugName) : Element(debugName) {
			this->pointAx = { 0.0, 0.0 };
			this->pointAy = { 0.0, 0.0 };
			this->pointBx = { 0.0, 1.0 };
			this->pointBy = { 0.0, 1.0 };
			this->thickness = { 0.0, 1.0 };
			this->fillColor = Color(255, 0, 0, 50);
			this->backgroundColor = Color(0, 0, 0, 0);
			this->offset = { 0.0, 0.0 };
		}

		LineShape(const std::string& debugName, AVec pointAx, AVec pointAy, AVec pointBx, AVec pointBy, AVec thickness, Color fillColor) : Element(debugName) {
			this->pointAx = pointAx;
			this->pointAy = pointAy;
			this->pointBx = pointBx;
			this->pointBy = pointBy;
			this->thickness = thickness;
			this->fillColor = fillColor;
			this->backgroundColor = Color(0, 0, 0, 0);
			this->offset = { 0.0, 0.0 };
		}

		void build(Batch& batch, Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";

			ScopedVertexArray scopedVertexArray(batch);
			
			// build the vertex array of own
			TriangleFan backgroundRect;
			if (backgroundColor.a > 0) { // with background fill
				backgroundRect.addVertex(scopedVertexArray.appendVertex(
					{ position.x, position.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex(
					{ position.x + size.x, position.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex(
					{ position.x + size.x, position.y + size.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex(
					{ position.x, position.y + size.y }, backgroundColor));

				backgroundRect.push(&scopedVertexArray);
			}

			TriangleFan lineShape;
			if (fillColor.a > 0) { // with background fill
				Vec2f delta = { pointBx.evaluate(size.x) - pointAx.evaluate(size.x), pointBy.evaluate(size.y) - pointAy.evaluate(size.y) };
				float distance = sqrt(delta.x * delta.x + delta.y * delta.y);

				lineShape.addVertex(scopedVertexArray.appendVertex(
					{ position.x + pointAx.evaluate(size.x) + delta.y * ((thickness.evaluate(min(size.x, size.y)) / 2 - offset.evaluate(min(size.x, size.y))) / distance),
						position.y + pointAy.evaluate(size.y) - delta.x * ((thickness.evaluate(min(size.x, size.y)) / 2 - offset.evaluate(min(size.x, size.y))) / distance) }, fillColor));
				lineShape.addVertex(scopedVertexArray.appendVertex(
					{ position.x + pointBx.evaluate(size.x) + delta.y * ((thickness.evaluate(min(size.x, size.y)) / 2 - offset.evaluate(min(size.x, size.y))) / distance),
						position.y + pointBy.evaluate(size.y) - delta.x * ((thickness.evaluate(min(size.x, size.y)) / 2 - offset.evaluate(min(size.x, size.y))) / distance) }, fillColor));
				lineShape.addVertex(scopedVertexArray.appendVertex(
					{ position.x + pointBx.evaluate(size.x) - delta.y * ((thickness.evaluate(min(size.x, size.y)) / 2 + offset.evaluate(min(size.x, size.y))) / distance),
						position.y + pointBy.evaluate(size.y) + delta.x * ((thickness.evaluate(min(size.x, size.y)) / 2 + offset.evaluate(min(size.x, size.y))) / distance) }, fillColor));
				lineShape.addVertex(scopedVertexArray.appendVertex(
					{ position.x + pointAx.evaluate(size.x) - delta.y * ((thickness.evaluate(min(size.x, size.y)) / 2 + offset.evaluate(min(size.x, size.y))) / distance),
						position.y + pointAy.evaluate(size.y) + delta.x * ((thickness.evaluate(min(size.x, size.y)) / 2 + offset.evaluate(min(size.x, size.y))) / distance) }, fillColor));

				lineShape.push(&scopedVertexArray);

			}

			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build(batch, position, size);

		}

	};
}