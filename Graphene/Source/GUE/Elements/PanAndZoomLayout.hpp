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
	/// Child bounds: 0.0 - 1.0
	/// Maximum children count: Unlimited
	///
	/// pan: child coordinate in the center
	/// zoom: 1.0 means when panned to (0.5, 0.5) it fits in the screen (ZOOM_TO_FIT)
	/// </summary>
	class PanAndZoomLayout : public Element {

	public:
		float panX;
		float panY;
		float zoom;
		Color fillColor;
		Color backgroundColor;

	private:

	public:
		PanAndZoomLayout(const std::string& debugName) : Element(debugName) {
			this->panX = 0.5;
			this->panY = 0.5;
			this->zoom = 1.0;
			this->fillColor = Color(0, 0, 0, 0);
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		PanAndZoomLayout(const std::string& debugName, Color fillColor) : Element(debugName) {
			this->panX = 0.5;
			this->panY = 0.5;
			this->zoom = 1.0;
			this->fillColor = fillColor;
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		void build(Batch& batch, Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";
			ScopedVertexArray scopedVertexArray(batch);

			// build the vertex array of own
			if (backgroundColor.a > 0) { // with background fill
				TriangleFan backgroundRect;

				backgroundRect.addVertex(scopedVertexArray.appendVertex(position, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x, position.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x, position.y + size.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x, position.y + size.y }, backgroundColor));

				backgroundRect.push(&scopedVertexArray);
			}

			if (fillColor.a > 0) {
				TriangleFan fillRect;

				fillRect.addVertex(scopedVertexArray.appendVertex({ position.x + leftPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y) }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x - rightPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y) }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x - rightPadding.evaluate(size.x), position.y + size.y - bottomPadding.evaluate(size.y) }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex({ position.x + leftPadding.evaluate(size.x), position.y + size.y - bottomPadding.evaluate(size.y) }, fillColor));

				fillRect.push(&scopedVertexArray);
			}

			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build(batch, { position.x + leftPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y) },
					{ size.x - leftPadding.evaluate(size.x) - rightPadding.evaluate(size.x), size.y - topPadding.evaluate(size.y) - bottomPadding.evaluate(size.y) });

		}

	};
}