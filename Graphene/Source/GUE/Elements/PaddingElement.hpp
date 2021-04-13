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
		ScopedVertexArray* m_scopedVertexArray = nullptr;

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
			this->topPadding = {0.0, 10.0f};
			this->leftPadding = { 0.0, 10.0f };
			this->bottomPadding = { 0.0, 10.0f };
			this->rightPadding = { 0.0, 10.0f };
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

			TriangleFan fillRect;
			if (fillColor.a > 0) { // with background fill
				fillRect.addVertex(m_scopedVertexArray->appendVertex({position.x + leftPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y)}, fillColor));
				fillRect.addVertex(m_scopedVertexArray->appendVertex({position.x + size.x - rightPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y)}, fillColor));
				fillRect.addVertex(m_scopedVertexArray->appendVertex({position.x + size.x - rightPadding.evaluate(size.x), position.y + size.y - bottomPadding.evaluate(size.y)}, fillColor));
				fillRect.addVertex(m_scopedVertexArray->appendVertex({position.x + leftPadding.evaluate(size.x), position.y + size.y - bottomPadding.evaluate(size.y)}, fillColor));
				
				fillRect.push(m_scopedVertexArray);
			}

			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
				(*child)->build({position.x + leftPadding.evaluate(size.x), position.y + topPadding.evaluate(size.y)},
					{size.x - leftPadding.evaluate(size.x) - rightPadding.evaluate(size.x), size.y - topPadding.evaluate(size.y) - bottomPadding.evaluate(size.y)});

		}

		void push(VertexArray* vertexArray) override {
			if (m_scopedVertexArray != nullptr) {
				//push scoped data to vertex array
				m_scopedVertexArray->pushToVertexArray(vertexArray);
				
				//recursively call chilren to push
				for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++)
					(*child)->push(vertexArray);
	
			}
		}

	};
}