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
	/// Child bounds: Depend on padding settings
	/// Maximum children count: Unlimited
	/// </summary>
	class PaddingElement : public Element {
		
	public:
		AVec topPadding;
		AVec leftPadding;
		AVec bottomPadding;
		AVec rightPadding;
		Color fillColor;
		Color backgroundColor;

	private:

	public:
		PaddingElement(const std::string& debugName) : Element(debugName) {
			this->topPadding = { 0.0, 10.0 };
			this->leftPadding = { 0.0, 10.0 };
			this->bottomPadding = { 0.0, 10.0 };
			this->rightPadding = { 0.0, 10.0 };
			this->fillColor = Color(0, 0, 0, 0);
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		PaddingElement(const std::string& debugName, AVec top, AVec left, AVec bottom, AVec right) : Element(debugName) {
			this->topPadding = top;
			this->leftPadding = left;
			this->bottomPadding = bottom;
			this->rightPadding = right;
			this->fillColor = Color(0, 0, 0, 0);
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		void build(VertexArray* vertexArray, Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";
			ScopedVertexArray scopedVertexArray(vertexArray);

			// build the vertex array of own
			if (backgroundColor.a > 0) { // with background fill
				TriangleFan backgroundRect;

				backgroundRect.addVertex(scopedVertexArray.appendVertex(position, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x, position.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x, position.y + size.y }, backgroundColor));
				backgroundRect.addVertex(scopedVertexArray.appendVertex({ position.x, position.y + size.y }, backgroundColor));

				backgroundRect.push(&scopedVertexArray);
			}

			if (fillColor.a > 0) { // with background fill
				TriangleFan fillRect;

				fillRect.addVertex(scopedVertexArray.appendVertex({ position.x + leftPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y) }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x - rightPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y) }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex({ position.x + size.x - rightPadding.evaluate(size.x), position.y + size.y - bottomPadding.evaluate(size.y) }, fillColor));
				fillRect.addVertex(scopedVertexArray.appendVertex({ position.x + leftPadding.evaluate(size.x), position.y + size.y - bottomPadding.evaluate(size.y) }, fillColor));

				fillRect.push(&scopedVertexArray);
			}

			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build(vertexArray, { position.x + leftPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y) },
					{ size.x - leftPadding.evaluate(size.x) - rightPadding.evaluate(size.x), size.y - topPadding.evaluate(size.y) - bottomPadding.evaluate(size.y) });

		}

	};
}