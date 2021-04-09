#pragma once
#define PI (float)3.141592

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Resources.hpp"

uniform_real_distribution<> disNorm(0.0, 1.0);

using namespace std;

class UI2 {
private:
	sf::RenderWindow* _window;
	Resources* _resources;

public:
	sf::RenderWindow* getWindow() {
		return _window;
	}

	void setWindow(sf::RenderWindow* window) {
		_window = window;
	}

	Resources* getResources() {
		return _resources;
	}

	void setResources(Resources* resources) {
		_resources = resources;
	}

	class VertexArray {
	public:
		struct Vertex {
			float _x;
			float _y;
			Vertex(float x, float y) {
				_x = x;
				_y = y;
			}
		};

	private:
		UI2* _ui;
		vector<sf::Vertex> vertexArray;

	public:
		VertexArray(UI2* ui) {
			_ui = ui;

		}

		void clear() {
			vertexArray.clear();
		}

		unsigned int getSize() {
			return vertexArray.size();
		}

		unsigned int getTriangleCount() {
			return getSize() / 3;
		}

		sf::Vertex* getBuffer() {
			return &vertexArray[0];
		}

		Vertex getVertex(unsigned int n) {
			return { vertexArray[n].position.x, vertexArray[n].position.y };
		}

		sf::Color getVertexColor(unsigned int n) {
			return vertexArray[n].color;
		}

		void appendDebugWireframe(Vertex v0, Vertex v1, float thickness, sf::Color color) {
			Vector2f delta = { v1._x - v0._x, v1._y - v0._y };
			float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
			vertexArray.push_back(sf::Vertex({ v0._x + delta.y * (thickness / 2 / distance), v0._y - delta.x * (thickness / 2 / distance) }, color));
			vertexArray.push_back(sf::Vertex({ v1._x + delta.y * (thickness / 2 / distance), v1._y - delta.x * (thickness / 2 / distance) }, color));
			vertexArray.push_back(sf::Vertex({ v1._x - delta.y * (thickness / 2 / distance), v1._y + delta.x * (thickness / 2 / distance) }, color));
			vertexArray.push_back(sf::Vertex({ v0._x + delta.y * (thickness / 2 / distance), v0._y - delta.x * (thickness / 2 / distance) }, color));
			vertexArray.push_back(sf::Vertex({ v1._x - delta.y * (thickness / 2 / distance), v1._y + delta.x * (thickness / 2 / distance) }, color));
			vertexArray.push_back(sf::Vertex({ v0._x - delta.y * (thickness / 2 / distance), v0._y + delta.x * (thickness / 2 / distance) }, color));
		}

		void appendTriangle(Vertex v0, Vertex v1, Vertex v2, sf::Color* color) {
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v0._x, v0._y), *color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v1._x, v1._y), *color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v2._x, v2._y), *color));

			if (_ui->_resources->showRenderingDebug) {
				appendDebugWireframe(v0, v1, 1, sf::Color(min(color->r + 50, 255), min(color->g + 50, 255), min(color->b + 50, 255)));
				appendDebugWireframe(v1, v2, 1, sf::Color(min(color->r + 50, 255), min(color->g + 50, 255), min(color->b + 50, 255)));
				appendDebugWireframe(v2, v0, 1, sf::Color(min(color->r + 50, 255), min(color->g + 50, 255), min(color->b + 50, 255)));
			}
		}

		void appendTriangle(Vertex v0, Vertex v1, Vertex v2, sf::Color color) {
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v0._x, v0._y), color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v1._x, v1._y), color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v2._x, v2._y), color));

			if (_ui->_resources->showRenderingDebug) {
				appendDebugWireframe(v0, v1, 1, sf::Color(min(color.r + 50, 255), min(color.g + 50, 255), min(color.b + 50, 255)));
				appendDebugWireframe(v1, v2, 1, sf::Color(min(color.r + 50, 255), min(color.g + 50, 255), min(color.b + 50, 255)));
				appendDebugWireframe(v2, v0, 1, sf::Color(min(color.r + 50, 255), min(color.g + 50, 255), min(color.b + 50, 255)));
			}
		}

		void appendQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3, sf::Color* color) {
			appendTriangle(v0, v1, v2, color);
			appendTriangle(v0, v2, v3, color);
		}

		void appendQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3, sf::Color color) {
			appendTriangle(v0, v1, v2, color);
			appendTriangle(v0, v2, v3, color);
		}

		void appendRectangle(Vector2f topLeftPosition, Vector2f size, sf::Color* color) {
			appendQuad({ topLeftPosition.x, topLeftPosition.y },
				{ topLeftPosition.x + size.x, topLeftPosition.y },
				{ topLeftPosition.x + size.x, topLeftPosition.y + size.y },
				{ topLeftPosition.x, topLeftPosition.y + size.y },
				color
			);
		}

		void appendRectangle(Vector2f topLeftPosition, Vector2f size, sf::Color color) {
			appendQuad({ topLeftPosition.x, topLeftPosition.y },
				{ topLeftPosition.x + size.x, topLeftPosition.y },
				{ topLeftPosition.x + size.x, topLeftPosition.y + size.y },
				{ topLeftPosition.x, topLeftPosition.y + size.y },
				color
			);
		}

		void appendLine(Vector2f v0, Vector2f v1, float thickness, float offset, sf::Color* color) {
			Vector2f delta = { v1.x - v0.x, v1.y - v0.y };
			float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
			appendQuad(
				{ v0.x + delta.y * ((thickness / 2 - offset) / distance), v0.y - delta.x * ((thickness / 2 - offset) / distance) },
				{ v1.x + delta.y * ((thickness / 2 - offset) / distance), v1.y - delta.x * ((thickness / 2 - offset) / distance) },
				{ v1.x - delta.y * ((thickness / 2 + offset) / distance), v1.y + delta.x * ((thickness / 2 + offset) / distance) },
				{ v0.x - delta.y * ((thickness / 2 + offset) / distance), v0.y + delta.x * ((thickness / 2 + offset) / distance) },
				color
			);
		}

		void appendLine(Vector2f v0, Vector2f v1, float thickness, float offset, sf::Color color) {
			Vector2f delta = { v1.x - v0.x, v1.y - v0.y };
			float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
			appendQuad(
				{ v0.x + delta.y * ((thickness / 2 - offset) / distance), v0.y - delta.x * ((thickness / 2 - offset) / distance) },
				{ v1.x + delta.y * ((thickness / 2 - offset) / distance), v1.y - delta.x * ((thickness / 2 - offset) / distance) },
				{ v1.x - delta.y * ((thickness / 2 + offset) / distance), v1.y + delta.x * ((thickness / 2 + offset) / distance) },
				{ v0.x - delta.y * ((thickness / 2 + offset) / distance), v0.y + delta.x * ((thickness / 2 + offset) / distance) },
				color
			);
		}

		void appendCircle(Vector2f centerPosition, float radius, sf::Color* color, unsigned int pointCount) {
			Vertex prevPoint = { centerPosition.x + radius * cos((float)1 / pointCount * 2 * PI),
				centerPosition.y + radius * sin((float)1 / pointCount * 2 * PI) };

			for (unsigned int p = 2; p < pointCount; p++) {
				Vertex thisPoint = { centerPosition.x + radius * cos((float)p / pointCount * 2 * PI),
					centerPosition.y + radius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ centerPosition.x + radius, centerPosition.y }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}
		}

		void appendCircle(Vector2f centerPosition, float radius, sf::Color color, unsigned int pointCount) {
			Vertex prevPoint = { centerPosition.x + radius * cos((float)1 / pointCount * 2 * PI),
				centerPosition.y + radius * sin((float)1 / pointCount * 2 * PI) };

			for (unsigned int p = 2; p < pointCount; p++) {
				Vertex thisPoint = { centerPosition.x + radius * cos((float)p / pointCount * 2 * PI),
					centerPosition.y + radius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ centerPosition.x + radius, centerPosition.y }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}
		}

		void appendRoundedRectangle(Vector2f topLeftPosition, Vector2f size, float cornerRadius, sf::Color* color, unsigned int pointCount) {
			Vertex prevPoint = { topLeftPosition.x + size.x, topLeftPosition.y + size.y - cornerRadius };

			for (unsigned int p = 1; p < pointCount / 4; p++) {
				Vertex thisPoint = { topLeftPosition.x + size.x - cornerRadius + cornerRadius * cos((float)p / pointCount * 2 * PI),
					topLeftPosition.y + size.y - cornerRadius + cornerRadius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}

			appendTriangle({ topLeftPosition.x + size.x,topLeftPosition.y + cornerRadius }, prevPoint,
				{ topLeftPosition.x + size.x - cornerRadius, topLeftPosition.y + size.y }, color);
			prevPoint = { topLeftPosition.x + size.x - cornerRadius, topLeftPosition.y + size.y };

			for (unsigned int p = pointCount / 4; p < 2 * pointCount / 4; p++) {
				Vertex thisPoint = { topLeftPosition.x + cornerRadius + cornerRadius * cos((float)p / pointCount * 2 * PI),
					topLeftPosition.y + size.y - cornerRadius + cornerRadius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}

			appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint,
				{ topLeftPosition.x, topLeftPosition.y + size.y - cornerRadius }, color);
			prevPoint = { topLeftPosition.x, topLeftPosition.y + size.y - cornerRadius };

			for (unsigned int p = 2 * pointCount / 4; p < 3 * pointCount / 4; p++) {
				Vertex thisPoint = { topLeftPosition.x + cornerRadius + cornerRadius * cos((float)p / pointCount * 2 * PI),
					topLeftPosition.y + cornerRadius + cornerRadius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}

			appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint,
				{ topLeftPosition.x + cornerRadius, topLeftPosition.y }, color);
			prevPoint = { topLeftPosition.x + cornerRadius, topLeftPosition.y };

			for (unsigned int p = 3 * pointCount / 4; p < pointCount; p++) {
				Vertex thisPoint = { topLeftPosition.x + size.x - cornerRadius + cornerRadius * cos((float)p / pointCount * 2 * PI),
					topLeftPosition.y + cornerRadius + cornerRadius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}

		}

		void appendRoundedRectangle(Vector2f topLeftPosition, Vector2f size, float cornerRadius, sf::Color color, unsigned int pointCount) {
			Vertex prevPoint = { topLeftPosition.x + size.x, topLeftPosition.y + size.y - cornerRadius };

			for (unsigned int p = 1; p < pointCount / 4; p++) {
				Vertex thisPoint = { topLeftPosition.x + size.x - cornerRadius + cornerRadius * cos((float)p / pointCount * 2 * PI),
					topLeftPosition.y + size.y - cornerRadius + cornerRadius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}

			appendTriangle({ topLeftPosition.x + size.x,topLeftPosition.y + cornerRadius }, prevPoint,
				{ topLeftPosition.x + size.x - cornerRadius, topLeftPosition.y + size.y }, color);
			prevPoint = { topLeftPosition.x + size.x - cornerRadius, topLeftPosition.y + size.y };

			for (unsigned int p = pointCount / 4; p < 2 * pointCount / 4; p++) {
				Vertex thisPoint = { topLeftPosition.x + cornerRadius + cornerRadius * cos((float)p / pointCount * 2 * PI),
					topLeftPosition.y + size.y - cornerRadius + cornerRadius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}

			appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint,
				{ topLeftPosition.x, topLeftPosition.y + size.y - cornerRadius }, color);
			prevPoint = { topLeftPosition.x, topLeftPosition.y + size.y - cornerRadius };

			for (unsigned int p = 2 * pointCount / 4; p < 3 * pointCount / 4; p++) {
				Vertex thisPoint = { topLeftPosition.x + cornerRadius + cornerRadius * cos((float)p / pointCount * 2 * PI),
					topLeftPosition.y + cornerRadius + cornerRadius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}

			appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint,
				{ topLeftPosition.x + cornerRadius, topLeftPosition.y }, color);
			prevPoint = { topLeftPosition.x + cornerRadius, topLeftPosition.y };

			for (unsigned int p = 3 * pointCount / 4; p < pointCount; p++) {
				Vertex thisPoint = { topLeftPosition.x + size.x - cornerRadius + cornerRadius * cos((float)p / pointCount * 2 * PI),
					topLeftPosition.y + cornerRadius + cornerRadius * sin((float)p / pointCount * 2 * PI) };
				appendTriangle({ topLeftPosition.x + size.x, topLeftPosition.y + cornerRadius }, prevPoint, thisPoint, color);
				prevPoint = thisPoint;
			}
		}
	};


public:
	class Element {
	public:
		struct AdaptiveVector {
		private:
			float _relativeComponent;
			float _absoluteComponent;

		public:
			AdaptiveVector() {
				_relativeComponent = 0;
				_absoluteComponent = 0;
			}

			AdaptiveVector(float relativeComponent, float absoluteComponent) {
				_relativeComponent = relativeComponent;
				_absoluteComponent = absoluteComponent;
			}

			void set(float relativeComponent, float absoluteComponent) {
				_relativeComponent = relativeComponent;
				_absoluteComponent = absoluteComponent;
			}

			void setRelativeComponent(float relativeComponent) {
				_relativeComponent = relativeComponent;
			}

			void setAbsoluteComponent(float absoluteComponent) {
				_absoluteComponent = absoluteComponent;
			}

			AdaptiveVector get() {
				return AdaptiveVector(_relativeComponent, _absoluteComponent);
			}

			float getRelativeComponent() {
				return _relativeComponent;
			}

			float getAbsoluteComponent() {
				return _absoluteComponent;
			}

			float evaluate(float parent) {
				return (parent * this->_relativeComponent + this->_absoluteComponent);
			}
		};

		enum class SizingMode {
			PER_AXIS,
			RELATIVE_TO_W,
			RELATIVE_TO_H,
			SHRINK_TO_FIT
		};

	private:
		UI2* _ui;
		Element* _parent;
		deque<Element*> _children;

		AdaptiveVector _x;
		AdaptiveVector _y;
		AdaptiveVector _w;
		AdaptiveVector _h;
		float _originX;
		float _originY;
		SizingMode _sizingMode;

		class Body {
		private:
			UI2* _ui;
			Element* _element;

		public:
			Body(UI2* ui, Element* parentElement) {
				_ui = ui;
				_element = parentElement;
			}

			enum class Type {
				NONE,
				CIRCLE,
				LINE,
				SIMPLE_TEXT,

			};

		private:
			Type _type;
			Color _backgroundColor = sf::Color(0, 0, 0, 0);
			string _debugName;

		public:
			Type getType() {
				return _type;
			}

			void setType(Type type) {
				_type = type;
			}

			string getDebugName() {
				return _debugName;
			}

			void setDebugName(string debugName) {
				_debugName = debugName;
			}

			Color getBackgroundColor() {
				return _backgroundColor;
			}

			void setBackgroundColor(Color* backgroundColor) {
				_backgroundColor = *backgroundColor;
			}

			void setBackgroundColor(Color backgroundColor) {
				_backgroundColor = backgroundColor;
			}


		public:

			// none


			// circle
			AdaptiveVector circleRadius = { 1.0, 0 };
			Color* circleFillColor;
			float circleOutlineThickness = 0;
			Color* circleOutlineColor;


			// rectangle


			// line
			AdaptiveVector linePointAX = { 0.0, 0 };
			AdaptiveVector linePointAY = { 0.0, 0 };
			AdaptiveVector linePointBX = { 1.0, 0 };
			AdaptiveVector linePointBY = { 1.0, 0 };
			float lineThickness = 1;
			Color* lineFillColor;
			float lineOutlineThickness = 0.0;
			Color* lineOutlineColor;
			float linePerpendicularOffset = 0;


			void setNone() {
				_type = Type::NONE;
			}

			void setCircle(AdaptiveVector _radius, Color* _fillColor) {
				_type = Type::CIRCLE;
				circleRadius = _radius;
				circleFillColor = _fillColor;
				circleOutlineThickness = 0;
				//circleOutlineColor = _ui->getResources()->colorTransparent;
			}

			void setCircle(AdaptiveVector _radius, Color* _fillColor, float _outlineThickness, Color* _outlineColor) {
				_type = Type::CIRCLE;
				circleRadius = _radius;
				circleFillColor = _fillColor;
				circleOutlineThickness = _outlineThickness;
				circleOutlineColor = _outlineColor;
			}

			void setLine(AdaptiveVector _aX, AdaptiveVector _aY, AdaptiveVector _bX, AdaptiveVector _bY, float _thickness, Color* _fillColor) {
				_type = Type::LINE;
				linePointAX = _aX;
				linePointAY = _aY;
				linePointBX = _bX;
				linePointBY = _bY;
				lineThickness = _thickness;
				lineFillColor = _fillColor;
				lineOutlineThickness = 0.0;
				//lineOutlineColor = _ui->getResources()->colorTransparent;
			}

			void setLine(AdaptiveVector _aX, AdaptiveVector _aY, AdaptiveVector _bX, AdaptiveVector _bY, float _thickness, Color* _fillColor,
				float _outlineThickness, Color* _outlineColor) {
				_type = Type::LINE;
				linePointAX = _aX;
				linePointAY = _aY;
				linePointBX = _bX;
				linePointBY = _bY;
				lineThickness = _thickness;
				lineFillColor = _fillColor;
				lineOutlineThickness = _outlineThickness;
				lineOutlineColor = _outlineColor;
			}

			/*void setSimpleText(string _str, Font* _font, float _size, Color* _color, float _alignmentX, float _alignmentY) {
				_type = Type::SIMPLE_TEXT;
				simpleTextString = _str;
				simpleTextFont = _font;
				simpleTextSize = _size;
				simpleTextColor = _color;
				simpleTextAlignmentX = _alignmentX;
				simpleTextAlignmentY = _alignmentY;
			}*/

			void renderBody(VertexArray* vertexArray, sf::Vector2f position, sf::Vector2f size) {

				vertexArray->appendRectangle(position, size, this->getBackgroundColor());


				switch (this->getType()) {
				case Type::NONE:

					break;

				case Type::CIRCLE:
					vertexArray->appendCircle(sf::Vector2f(position.x + size.x / 2, position.y + size.y / 2), circleRadius.evaluate(size.x), circleFillColor, 40);
					break;

				case Type::LINE:
					vertexArray->appendLine(
						sf::Vector2f(position.x + linePointAX.evaluate(size.x), position.y + linePointAY.evaluate(size.y)),
						sf::Vector2f(position.x + linePointBX.evaluate(size.x), position.y + linePointBY.evaluate(size.y)),
						lineThickness,
						linePerpendicularOffset,
						lineFillColor
					);
					break;

				case Type::SIMPLE_TEXT:

					break;

				default:
					break;
				}
			}


		};

		Body* _body = new Body(_ui, this);
		string _debugName;

	public:
		Element(UI2* ui, Element* parent, string debugName) {
			_ui = ui;
			_parent = parent;
			_debugName = debugName;
			_body->setDebugName(debugName);

			if (parent != nullptr)
				parent->_children.push_back(this);
			_x = { 0, 0 };
			_y = { 0, 0 };
			_w = { 1, 0 };
			_h = { 1, 0 };
			_originX = 0;
			_originY = 0;
			_sizingMode = SizingMode::PER_AXIS;

		}

		Element(UI2* ui, Element* parent, string debugName, AdaptiveVector x, AdaptiveVector y, AdaptiveVector w, AdaptiveVector h, float originX, float originY) {
			_ui = ui;
			_parent = parent;
			_debugName = debugName;
			_body->setDebugName(debugName);

			if (parent != nullptr)
				parent->_children.push_back(this);
			_x = x;
			_y = y;
			_w = w;
			_h = h;
			_originX = originX;
			_originY = originY;
			_sizingMode = SizingMode::PER_AXIS;
		}

		~Element() {
			delete this->_body;
			for (deque<Element*>::iterator child = this->_children.begin(); child != this->_children.end(); child++) {
				delete (*child);
			}
		}

		void deleteElement() {
			if (this == nullptr)
				return;
			if (this->_parent != nullptr)
				this->_parent->_children.erase(find(this->_parent->_children.begin(), this->_parent->_children.end(), this));
			delete this;
		}

		void linkContainer(Element* parent) {
			if (this == _parent)
				return;
			if (this->_parent != nullptr)
				this->_parent->_children.erase(find(this->_parent->_children.begin(), this->_parent->_children.end(), this));
			this->_parent = parent;
			this->_parent->_children.push_back(this);
		}

		void unlinkContainer() {
			if (this == this->_parent)
				return;
			this->_parent->_children.erase(remove(this->_parent->_children.begin(), this->_parent->_children.end(), this), this->_parent->_children.end());
			this->_parent = this;
		}

		Element* getParentElement() {
			return _parent;
		}

		deque<Element*>* getChildren() {
			return &_children;
		}

		AdaptiveVector getX() {
			return _x;
		}

		void setX(float relativeComponent, float absoluteComponent) {
			_x.set(relativeComponent, absoluteComponent);
		}

		void setX(AdaptiveVector x) {
			_x = x;
		}

		float evalX(float parent) {
			return _x.evaluate(parent);
		}

		AdaptiveVector getY() {
			return _y;
		}

		void setY(float relativeComponent, float absoluteComponent) {
			_y.set(relativeComponent, absoluteComponent);
		}

		void setY(AdaptiveVector y) {
			_y = y;
		}

		float evalY(float parent) {
			return _y.evaluate(parent);
		}

		AdaptiveVector getW() {
			return _w;
		}

		void setW(float relativeComponent, float absoluteComponent) {
			_w.set(relativeComponent, absoluteComponent);
		}

		void setW(AdaptiveVector w) {
			_w = w;
		}

		float evalW(float parent) {
			return _w.evaluate(parent);
		}

		AdaptiveVector getH() {
			return _h;
		}

		void setH(float relativeComponent, float absoluteComponent) {
			_h.set(relativeComponent, absoluteComponent);
		}

		void setH(AdaptiveVector h) {
			_h = h;
		}

		float evalH(float parent) {
			return _h.evaluate(parent);
		}

		float getOriginX() {
			return _originX;
		}

		void setOriginX(float originX) {
			_originX = originX;
		}

		float evalOriginX(float parent) {
			return _originX * parent;
		}

		float getOriginY() {
			return _originY;
		}

		void setOriginY(float originY) {
			_originY = originY;
		}

		float evalOriginY(float parent) {
			return _originY * parent;
		}

		SizingMode getSizingMode() {
			return _sizingMode;
		}

		void setSizingMode(SizingMode sizingMode) {
			_sizingMode = sizingMode;
		}

		Body* getBody() {
			return _body;
		}

		string getDebugName() {
			return _debugName;
		}

		void setDebugName(string debugName) {
			_debugName = debugName;
		}

		


	};


	class LayoutGenerator {
	private:
		UI2* _ui;
		VertexArray* _vertexArray;

	public:
		LayoutGenerator(UI2* ui) {
			_ui = ui;
			_vertexArray = new VertexArray(_ui);

		}


		

		void generateSublayout(Element* thisElement, Vector2f thisPosition, Vector2f thisSize) {
			// TODO: body rendering
			thisElement->getBody()->renderBody(getVertexArray(), thisPosition, thisSize);
			
			for (deque<Element*>::iterator child = thisElement->getChildren()->begin();
				child != thisElement->getChildren()->end(); child++) {

				sf::Vector2f childSize((*child)->evalW(thisSize.x), (*child)->evalH(thisSize.y));

				if ((*child)->getSizingMode() == Element::SizingMode::RELATIVE_TO_H)
					childSize.x = (*child)->evalW(childSize.y);
				if ((*child)->getSizingMode() == Element::SizingMode::RELATIVE_TO_W)
					childSize.y = (*child)->evalH(childSize.x);
				if ((*child)->getSizingMode() == Element::SizingMode::SHRINK_TO_FIT) {
					childSize = Vector2f(
						childSize.x / max(childSize.x / thisSize.x, childSize.y / thisSize.y),
						childSize.y / max(childSize.x / thisSize.x, childSize.y / thisSize.y)
					);
				}

				sf::Vector2f childPosition(thisPosition.x + (*child)->evalX(thisSize.x) - (*child)->evalOriginX(childSize.x),
					thisPosition.y + (*child)->evalY(thisSize.y) - (*child)->evalOriginY(childSize.y));

				generateSublayout(*child, childPosition, childSize);
			}
		}

	public:
		void generateLayout(Element* rootElement, Vector2f topLeftCoord, Vector2f size) {
			_vertexArray->clear();
			generateSublayout(rootElement, topLeftCoord, size);
		}

		VertexArray* getVertexArray() {
			return _vertexArray;
		}

		~LayoutGenerator() {
			delete _vertexArray;
		}

	};

public:
	UI2(sf::RenderWindow* window, Resources* resources) {
		_window = window;
		_resources = resources;
	}


private:
	Element* _rootElement = new Element(this, nullptr, "rootElement");

public:
	Element* getRootElement() {
		return _rootElement;
	}


public:

	void renderUI() {
		sf::Clock profilerClock;
		profilerClock.restart();

		_window->clear(sf::Color(0, 0, 0));

		LayoutGenerator layoutGenerator(this);
		//cout << "Spawn layout generator: " << profilerClock.getElapsedTime().asMicroseconds() << "\n";
		profilerClock.restart();

		layoutGenerator.generateLayout(_rootElement, Vector2f(0, 0), Vector2f((float)_window->getSize().x, (float)_window->getSize().y));
		//cout << "       Generate layout: " << profilerClock.getElapsedTime().asMicroseconds() << "\n";
		profilerClock.restart();

		cout << "\t\t\t\t\tTriangles: " << layoutGenerator.getVertexArray()->getTriangleCount() << "\n";

		_window->draw(layoutGenerator.getVertexArray()->getBuffer(), layoutGenerator.getVertexArray()->getSize(), sf::Triangles);
		//cout << "                  Draw: " << profilerClock.getElapsedTime().asMicroseconds() << "\n";
		profilerClock.restart();

		_window->display();
		//cout << "               Display: " << profilerClock.getElapsedTime().asMicroseconds() << "\n";

	}


};