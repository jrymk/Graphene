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
	/// Children type: Line shape with same color only
	/// Maximum children count: Unlimited
	/// </summary>
	class LineSetShape : public Element {

	public:

		enum class LineJoin {
			NONE,
			BEVEL,
			ROUND,
			MITER,
		};

		enum class LineCap {
			NONE,
			SQUARE,
			ROUND,
			TRIANGLE,
		};

		LineJoin lineJoin;
		LineCap lineCap;
		
		Color backgroundColor;
		

	private:

	public:
		LineSetShape(const std::string& debugName) : Element(debugName) {
			this->backgroundColor = Color(0, 0, 0, 0);
		}

		void build(VertexArray& vertexArray, Vec2f position, Vec2f size) override {
			//std::cout << debugName << "\n";

			BatchAllocator batch(vertexArray);
			batch.beginAlloc();

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


			//recursively call chilren to build
			for (auto child = m_childrenElements.begin(); child != m_childrenElements.end(); child++) {

				//TODO: add line join and line cap drawing here
				
				(*child)->build(batch, position, size);
			}

		}

	};
}