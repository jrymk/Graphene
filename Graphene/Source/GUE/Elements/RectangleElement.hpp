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
		ScopedVertexArray* m_scopedVertexArray = nullptr;

	public:
		AVec x;
		AVec y;
		AVec w;
		AVec h;
		Color fillColor;
		Color backgroundColor;

	private:

	public:
		RectangleElement(const std::string& debugName) : Element(debugName) {
			this->x = { 0.0, 0.0 };
			this->y = { 0.0, 0.0 };
			this->w = { 0.0, 1.0 };
			this->h = { 0.0, 1.0 };
			this->fillColor = Color(255, 0, 0, 50);
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		RectangleElement(const std::string& debugName, AVec x, AVec y, AVec w, AVec h, Color fillColor) : Element(debugName) {
			this->x = x;
			this->y = y;
			this->w = h;
			this->h = w;
			this->fillColor = fillColor;
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		void build(Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";
			m_scopedVertexArray = new ScopedVertexArray();

			// build the vertex array of own
			TriangleFan backgroundRect;
			if (backgroundColor.a > 0) { // with background fill
				backgroundRect.addVertex(m_scopedVertexArray->appendVertex(
					{   position.x,            position.y },          backgroundColor));
				backgroundRect.addVertex(m_scopedVertexArray->appendVertex(
					{ position.x + size.x,   position.y },          backgroundColor));
				backgroundRect.addVertex(m_scopedVertexArray->appendVertex(
					{ position.x + size.x, position.y + size.y }, backgroundColor));
				backgroundRect.addVertex(m_scopedVertexArray->appendVertex(
					{   position.x,          position.y + size.y }, backgroundColor));

				backgroundRect.push(m_scopedVertexArray);
			}

			TriangleFan fillRect;
			if (fillColor.a > 0) { // with background fill
				fillRect.addVertex(m_scopedVertexArray->appendVertex(
					{ position.x + x.evaluate(size.x),                      position.y + y.evaluate(size.y) }, fillColor));
				fillRect.addVertex(m_scopedVertexArray->appendVertex(
					{ position.x + x.evaluate(size.x) + w.evaluate(size.x), position.y + y.evaluate(size.y) }, fillColor));
				fillRect.addVertex(m_scopedVertexArray->appendVertex(
					{ position.x + x.evaluate(size.x) + w.evaluate(size.x), position.y + y.evaluate(size.y) + h.evaluate(size.y) }, fillColor));
				fillRect.addVertex(m_scopedVertexArray->appendVertex(
					{ position.x + x.evaluate(size.x),                      position.y + y.evaluate(size.y) + h.evaluate(size.y) }, fillColor));

				fillRect.push(m_scopedVertexArray);
			}

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