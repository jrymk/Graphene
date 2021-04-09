#pragma once
#define PI (float)3.141592

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <random>
#include <utility>

#include "Resources.hpp"

uniform_real_distribution<> disNorm(0.0, 1.0);

using namespace std;

class Engine {
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
		Engine* _ui;
		vector<sf::Vertex> vertexArray;

	public:
		VertexArray(Engine* ui) {
			_ui = ui;

		}

		void clear() {
			vertexArray.clear();
		}

		unsigned long long getSize() {
			return vertexArray.size();
		}

		unsigned long long getTriangleCount() {
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

			if (_ui->resources_->showRenderingDebug) {
				appendDebugWireframe(v0, v1, 1, sf::Color(min(color->r + 20, 255), min(color->g + 20, 255), min(color->b + 20, 255)));
				appendDebugWireframe(v1, v2, 1, sf::Color(min(color->r + 20, 255), min(color->g + 20, 255), min(color->b + 20, 255)));
				appendDebugWireframe(v2, v0, 1, sf::Color(min(color->r + 20, 255), min(color->g + 20, 255), min(color->b + 20, 255)));
			}
		}

		void appendTriangle(Vertex v0, Vertex v1, Vertex v2, sf::Color color) {
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v0._x, v0._y), color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v1._x, v1._y), color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v2._x, v2._y), color));

			if (_ui->resources_->showRenderingDebug) {
				appendDebugWireframe(v0, v1, 1, sf::Color(min(color.r + 20, 255), min(color.g + 20, 255), min(color.b + 20, 255)));
				appendDebugWireframe(v1, v2, 1, sf::Color(min(color.r + 20, 255), min(color.g + 20, 255), min(color.b + 20, 255)));
				appendDebugWireframe(v2, v0, 1, sf::Color(min(color.r + 20, 255), min(color.g + 20, 255), min(color.b + 20, 255)));
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

	sf::RenderWindow* window_;
	Resources* resources_;

public:
	sf::RenderWindow* getWindow() {
		return window_;
	}

	void setWindow(sf::RenderWindow* window) {
		window_ = window;
	}

	Resources* getResources() {
		return resources_;
	}

	void setResources(Resources* resources) {
		resources_ = resources;
	}


	class Element {
	public:
		struct AdaptiveVector {
		private:
			float rel_;
			float abs_;

		public:
			AdaptiveVector() {
				rel_ = 0;
				abs_ = 0;
			}

			AdaptiveVector(float rel, float abs) {
				rel_ = rel;
				abs_ = abs;
			}

			void set(float rel, float abs) {
				rel_ = rel;
				abs_ = abs;
			}

			void setRel(float rel) {
				rel_ = rel;
			}

			void setAbs(float abs) {
				abs_ = abs;
			}

			AdaptiveVector get() {
				return { rel_, abs_ };
			}

			float getRel() {
				return rel_;
			}

			float getAbs() {
				return abs_;
			}

			float eval(float parent) {
				return (parent * this->rel_ + this->abs_);
			}
		};

		enum class SizingMode {
			PER_AXIS,
			RELATIVE_TO_W,
			RELATIVE_TO_H,
			SHRINK_TO_FIT
		};

	private:
		Engine* engine_;
		Element* parent_;
		Element* firstChild_;
		Element* previousElement_;
		Element* nextElement_;
		deque<Element*> children_;

		AdaptiveVector x_;
		AdaptiveVector y_;
		AdaptiveVector w_;
		AdaptiveVector h_;
		float originX_;
		float originY_;
		SizingMode sizingMode_;

		class Body {
		private:
			Engine* _ui;
			Element* _element;

		public:
			Body(Engine* ui, Element* parentElement) {
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
				_debugName = std::move(debugName);
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
					vertexArray->appendCircle(sf::Vector2f(position.x + size.x / 2, position.y + size.y / 2), circleRadius.eval(size.x), circleFillColor, 40);
					break;

				case Type::LINE:
					vertexArray->appendLine(
						sf::Vector2f(position.x + linePointAX.eval(size.x), position.y + linePointAY.eval(size.y)),
						sf::Vector2f(position.x + linePointBX.eval(size.x), position.y + linePointBY.eval(size.y)),
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

		Body* body_;
		string debugName_;

	public:
		Element(Engine* engine, Element* parent, const string& debugName) {
			engine_ = engine;
			parent_ = parent;
			debugName_ = debugName;

			body_ = new Body(engine_, this);
			body_->setDebugName(debugName);

			if (parent != nullptr)
				parent->children_.push_back(this);
			x_ = { 0, 0 };
			y_ = { 0, 0 };
			w_ = { 1, 0 };
			h_ = { 1, 0 };
			originX_ = 0;
			originY_ = 0;
			sizingMode_ = SizingMode::PER_AXIS;
		}

		Element(Engine* engine, Element* parent, const string& debugName, AdaptiveVector x, AdaptiveVector y, AdaptiveVector w, AdaptiveVector h, float originX, float originY) {
			engine_ = engine;
			parent_ = parent;
			debugName_ = debugName;

			body_ = new Body(engine_, this);
			body_->setDebugName(debugName);

			if (parent != nullptr)
				parent->children_.push_back(this);
			x_ = x;
			y_ = y;
			w_ = w;
			h_ = h;
			originX_ = originX;
			originY_ = originY;
			sizingMode_ = SizingMode::PER_AXIS;
		}

		~Element() {
			delete this->body_;

			for (deque<Element*>::iterator child = this->children_.begin(); child != this->children_.end(); child++) {
				delete (*child);
			}
		}

		void deleteElement() {
			if (this->parent_ != nullptr)
				this->parent_->children_.erase(find(this->parent_->children_.begin(), this->parent_->children_.end(), this));

			delete this;
		}

		/*void insertChildAfter(Element* child, Element* after) {
			//this->children_.insert(child, )
		}*/

		void linkParentElement(Element* parent) {
			if (this == parent)
				return;

			if (this->parent_ != nullptr)
				this->parent_->children_.erase(find(this->parent_->children_.begin(), this->parent_->children_.end(), this));

			this->parent_ = parent;
			if (parent != nullptr)
				this->parent_->children_.push_back(this);
		}

		void unlinkParentElement() {
			if (this->parent_ == nullptr)
				return;

			this->parent_->children_.erase(remove(this->parent_->children_.begin(), this->parent_->children_.end(), this), this->parent_->children_.end());
			this->parent_ = nullptr;
		}

		Element* getParentElement() {
			return parent_;
		}

		deque<Element*>* getChildren() {
			return &children_;
		}

		AdaptiveVector getX() {
			return x_;
		}

		void setX(float rel, float abs) {
			x_.set(rel, abs);
		}

		void setX(AdaptiveVector x) {
			x_ = x;
		}

		float evalX(float parent) {
			return x_.eval(parent);
		}

		AdaptiveVector getY() {
			return y_;
		}

		void setY(float rel, float abs) {
			y_.set(rel, abs);
		}

		void setY(AdaptiveVector y) {
			y_ = y;
		}

		float evalY(float parent) {
			return y_.eval(parent);
		}

		AdaptiveVector getW() {
			return w_;
		}

		void setW(float rel, float abs) {
			w_.set(rel, abs);
		}

		void setW(AdaptiveVector w) {
			w_ = w;
		}

		float evalW(float parent) {
			return w_.eval(parent);
		}

		AdaptiveVector getH() {
			return h_;
		}

		void setH(float rel, float abs) {
			h_.set(rel, abs);
		}

		void setH(AdaptiveVector h) {
			h_ = h;
		}

		float evalH(float parent) {
			return h_.eval(parent);
		}

		float getOriginX() {
			return originX_;
		}

		void setOriginX(float originX) {
			originX_ = originX;
		}

		float evalOriginX(float parent) {
			return originX_ * parent;
		}

		float getOriginY() {
			return originY_;
		}

		void setOriginY(float originY) {
			originY_ = originY;
		}

		float evalOriginY(float parent) {
			return originY_ * parent;
		}

		SizingMode getSizingMode() {
			return sizingMode_;
		}

		void setSizingMode(SizingMode sizingMode) {
			sizingMode_ = sizingMode;
		}

		Body* getBody() {
			return body_;
		}

		void resetBody() {
			delete body_;
			body_ = new Body(engine_, this);
		}

		string getDebugName() {
			return debugName_;
		}

		void setDebugName(string debugName) {
			debugName_ = std::move(debugName);
		}

	};


	class LayoutGenerator {
		Engine* engine_;
		VertexArray* vertexArray_;

	public:
		LayoutGenerator(Engine* engine) {
			engine_ = engine;
			vertexArray_ = new VertexArray(engine);

		}

		void generateSublayout(Element* thisElement, Vector2f thisPosition, Vector2f thisSize) {
			// TODO: body rendering
			thisElement->getBody()->renderBody(getVertexArray(), thisPosition, thisSize);

			for (deque<Element*>::iterator child = thisElement->getChildren()->begin(); child != thisElement->getChildren()->end(); child++) {
				sf::Vector2f childSize((*child)->evalW(thisSize.x), (*child)->evalH(thisSize.y));

				switch ((*child)->getSizingMode()) {
				case Element::SizingMode::PER_AXIS:
					
					break;

				case Element::SizingMode::RELATIVE_TO_H:
					childSize.x = (*child)->evalW(childSize.y);
					break;

				case Element::SizingMode::RELATIVE_TO_W:
					childSize.y = (*child)->evalH(childSize.x);
					break;

				case Element::SizingMode::SHRINK_TO_FIT:
					childSize = Vector2f(
						childSize.x / max(childSize.x / thisSize.x, childSize.y / thisSize.y),
						childSize.y / max(childSize.x / thisSize.x, childSize.y / thisSize.y)
					);
					break;

				}

				sf::Vector2f childPosition(
					thisPosition.x + (*child)->evalX(thisSize.x) - (*child)->evalOriginX(childSize.x),
					thisPosition.y + (*child)->evalY(thisSize.y) - (*child)->evalOriginY(childSize.y)
				);

				generateSublayout(*child, childPosition, childSize);
			}
		}

		void generateLayout(Element* rootElement, Vector2f topLeftCoord, Vector2f size) {
			vertexArray_->clear();
			generateSublayout(rootElement, topLeftCoord, size);
		}

		VertexArray* getVertexArray() {
			return vertexArray_;
		}

		~LayoutGenerator() {
			delete vertexArray_;
		}

	};


	Element* rootElement_;

	Engine(sf::RenderWindow* window, Resources* resources) {
		window_ = window;
		resources_ = resources;

		rootElement_ = new Element(this, nullptr, "rootElement");
	}

	Element* getRootElement() {
		return rootElement_;
	}

	void resetRootElement() {
		rootElement_->deleteElement();
		rootElement_ = new Element(this, nullptr, "rootElement");
	}

	void render() {
		sf::Clock profilerClock;
		profilerClock.restart();

		window_->clear(sf::Color(0, 0, 0));

		LayoutGenerator layoutGenerator(this);
		//cout << "Spawn layout generator: " << profilerClock.getElapsedTime().asMicroseconds() << "\n";
		profilerClock.restart();

		layoutGenerator.generateLayout(rootElement_, Vector2f(0, 0), Vector2f((float)window_->getSize().x, (float)window_->getSize().y));
		//cout << "       Generate layout: " << profilerClock.getElapsedTime().asMicroseconds() << "\n";
		profilerClock.restart();

		cout << "Triangles: " << layoutGenerator.getVertexArray()->getTriangleCount() << "\n";

		window_->draw(layoutGenerator.getVertexArray()->getBuffer(), layoutGenerator.getVertexArray()->getSize(), sf::Triangles);
		//cout << "                  Draw: " << profilerClock.getElapsedTime().asMicroseconds() << "\n";
		profilerClock.restart();

		window_->display();
		//cout << "               Display: " << profilerClock.getElapsedTime().asMicroseconds() << "\n";

	}


};