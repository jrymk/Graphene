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
	/// Maximum children count: Not recommended
	/// </summary>
	class RectangleElement : public Element {

	public:

		enum class SizingMode {
			NONE,
			PER_AXIS,
			HEIGHT_RELATIVE_TO_WIDTH,
			WIDTH_RELATIVE_TO_HEIGHT,
			SHRINK_TO_FIT,

		};

	public:
		AVec x;
		AVec y;
		AVec w;
		AVec h;
		SizingMode sizingMode;
		Color fillColor;
		Color backgroundColor;

	private:

	public:
		RectangleElement(const std::string& debugName) : Element(debugName) {
			this->x = { 0.0, 0.0 };
			this->y = { 0.0, 0.0 };
			this->w = { 0.0, 1.0 };
			this->h = { 0.0, 1.0 };
			this->sizingMode = SizingMode::PER_AXIS;
			this->fillColor = Color(255, 0, 0, 50);
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		RectangleElement(const std::string& debugName, AVec x, AVec y, AVec w, AVec h, Color fillColor) : Element(debugName) {
			this->x = x;
			this->y = y;
			this->w = h;
			this->h = w;
			this->sizingMode = SizingMode::PER_AXIS;
			this->fillColor = fillColor;
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		void build(Batch& batch, Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";
			ScopedVertexArray scopedVertexArray(batch);

			// build the vertex array of own

			if (backgroundColor.a > 0) { // with background fill

				TriangleFan backgroundRect;

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



			if (fillColor.a > 0) { // with shape fill
				
				float width = w.evaluate(size.x);
				float height = h.evaluate(size.y);

				switch (sizingMode) {
				case SizingMode::NONE:
					width = w.evaluate(0);
					height = h.evaluate(0);
					break;

				case SizingMode::PER_AXIS:
					break;

				case SizingMode::HEIGHT_RELATIVE_TO_WIDTH:
					height = h.evaluate(width);
					break;

				case SizingMode::WIDTH_RELATIVE_TO_HEIGHT:
					width = w.evaluate(height);
					break;

				case SizingMode::SHRINK_TO_FIT:
					const float shrinkRatio = max(width / size.x, height / size.y);
					width /= shrinkRatio;
					height /= shrinkRatio;
					break;
				}

				
				TriangleFan fillRect;

				fillRect.addVertex(scopedVertexArray.appendVertex(
					{ position.x + x.evaluate(size.x), position.y + y.evaluate(size.y) }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex(
					{ position.x + x.evaluate(size.x) + width, position.y + y.evaluate(size.y) }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex(
					{ position.x + x.evaluate(size.x) + width, position.y + y.evaluate(size.y) + height }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex(
					{ position.x + x.evaluate(size.x), position.y + y.evaluate(size.y) + height }, fillColor));

				fillRect.push(&scopedVertexArray);
				
			}

			
			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build(batch, position, size);

		}

	};
}