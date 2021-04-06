#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <set>
#include "ExceptionHandler.hpp"
#include "Resources.hpp"
#include <random>
#include <algorithm>

using namespace std;
using namespace sf;

uniform_int_distribution<> dis255(0, 255);

class UI {
private:
	ExceptionHandler eh;

public:
	struct AdaptiveVector {
		float relativeComponent;
		float absoluteComponent;

		AdaptiveVector() {
			relativeComponent = 0.0;
			absoluteComponent = 0;
		}

		AdaptiveVector(float _relativeComponent, float _absoluteComponent) {
			relativeComponent = _relativeComponent;
			absoluteComponent = _absoluteComponent;
		}

		void set(float _relativeComponent, float _absoluteComponent) {
			relativeComponent = _relativeComponent;
			absoluteComponent = _absoluteComponent;
		}

		void fillParent() {
			relativeComponent = 1.0;
			absoluteComponent = 0;
		}

		void parentRatio(float ratio) {
			relativeComponent = ratio;
			absoluteComponent = 0;
		}

		void absolute(float value) {
			relativeComponent = 0.0;
			absoluteComponent = value;
		}

		void reverseAbsolute(float value) {
			relativeComponent = 1.0;
			absoluteComponent = value;
		}

		double evaluate(float parent) {
			return (parent * this->relativeComponent + this->absoluteComponent);
		}

	};

	class Element {
	private:
		ExceptionHandler eh;

	public:

		class Body {
		public:
			enum class Type {
				NONE,
				CIRCLE,
				LINE,
				SIMPLE_TEXT,

			};

		public:
			Resources resources;
			Type type = Type::NONE;
			Color* backgroundColor = resources.colorTransparent;
			string debugName;

			// none


			// circle
			CircleShape circle;
			AdaptiveVector circleRadius = { 1.0, 0 };
			Color* circleFillColor = resources.colorTransparent;
			float circleOutlineThickness = 0;
			Color* circleOutlineColor = resources.colorTransparent;

			// rectangle


			// line
			RectangleShape lineRect;
			AdaptiveVector linePointAX = { 0.0, 0 };
			AdaptiveVector linePointAY = { 0.0, 0 };
			AdaptiveVector linePointBX = { 1.0, 0 };
			AdaptiveVector linePointBY = { 1.0, 0 };
			float lineThickness = 1;
			Color* lineFillColor = resources.colorTransparent;
			float lineOutlineThickness = 0.0;
			Color* lineOutlineColor = resources.colorTransparent;
			float linePerpendicularOffset = 0;
			float lineDeltaX = 0.0;
			float lineDeltaY = 0.0;

			// simple text
			Font* simpleTextFont = resources.fontDefault;
			Text simpleText;
			string simpleTextString;
			float simpleTextSize = 0;
			Color* simpleTextColor = resources.colorTransparent;
			float simpleTextAlignmentX = 0.5;
			float simpleTextAlignmentY = 0.5;
			float simpleTextTempBoundsLeft = 0.0;
			float simpleTextTempBoundsTop = 0.0;


		public:
			Body() {
				type = Type::NONE;
			}

			void setBackgroundColor(Color* _color) {
				backgroundColor = _color;
			}

			void setBackgroundColor(Color _color) {
				backgroundColor = &_color;
			}

			void setNone() {
				type = Type::NONE;
			}

			void setCircle(AdaptiveVector _radius, Color* _fillColor) {
				type = Type::CIRCLE;
				circleRadius = _radius;
				circleFillColor = _fillColor;
				circleOutlineThickness = 0;
				circleOutlineColor = resources.colorTransparent;
			}

			void setCircle(AdaptiveVector _radius, Color* _fillColor, float _outlineThickness, Color* _outlineColor) {
				type = Type::CIRCLE;
				circleRadius = _radius;
				circleFillColor = _fillColor;
				circleOutlineThickness = _outlineThickness;
				circleOutlineColor = _outlineColor;
			}

			void setLine(AdaptiveVector _aX, AdaptiveVector _aY, AdaptiveVector _bX, AdaptiveVector _bY, float _thickness, Color* _fillColor) {
				type = Type::LINE;
				linePointAX = _aX;
				linePointAY = _aY;
				linePointBX = _bX;
				linePointBY = _bY;
				lineThickness = _thickness;
				lineFillColor = _fillColor;
				lineOutlineThickness = 0.0;
				lineOutlineColor = resources.colorTransparent;
			}

			void setLine(AdaptiveVector _aX, AdaptiveVector _aY, AdaptiveVector _bX, AdaptiveVector _bY, float _thickness, Color* _fillColor,
				float _outlineThickness, Color* _outlineColor) {
				type = Type::LINE;
				linePointAX = _aX;
				linePointAY = _aY;
				linePointBX = _bX;
				linePointBY = _bY;
				lineThickness = _thickness;
				lineFillColor = _fillColor;
				lineOutlineThickness = _outlineThickness;
				lineOutlineColor = _outlineColor;
			}

			void setSimpleText(string _str, Font* _font, float _size, Color* _color, float _alignmentX, float _alignmentY) {
				type = Type::SIMPLE_TEXT;
				simpleTextString = _str;
				simpleTextFont = _font;
				simpleTextSize = _size;
				simpleTextColor = _color;
				simpleTextAlignmentX = _alignmentX;
				simpleTextAlignmentY = _alignmentY;
			}

			void render(RenderTexture* globalTexture, Vector2f thisPosition, Vector2f thisSize) {
				RectangleShape rectShape;
				if (resources.showDebugBoundaries) {
					rectShape.setOutlineColor(*resources.colorUIBoundsDebug);
					rectShape.setOutlineThickness(-1);
				}
				else {
					rectShape.setOutlineColor(*resources.colorTransparent);
					rectShape.setOutlineThickness(0);
				}
				rectShape.setSize(thisSize);
				rectShape.setPosition(thisPosition);

				rectShape.setFillColor(*(this->backgroundColor));

				globalTexture->draw(rectShape);

				switch (this->type) {
				case Type::NONE:
					break;

				case Type::CIRCLE:
					circle.setRadius(circleRadius.evaluate(min(thisSize.x, thisSize.y)));
					circle.setFillColor(*circleFillColor);
					circle.setOutlineThickness(circleOutlineThickness);
					circle.setOutlineColor(*circleOutlineColor);
					circle.setPosition(Vector2f(
						thisPosition.x + AdaptiveVector(0.5, 0).evaluate(thisSize.x) - circle.getRadius(),
						thisPosition.y + AdaptiveVector(0.5, 0).evaluate(thisSize.y) - circle.getRadius()
					));
					globalTexture->draw(circle);

					if (resources.showDebugBoundaries) {
						RectangleShape rectShape;
						rectShape.setFillColor(*resources.colorTransparent);
						rectShape.setOutlineColor(*resources.colorUIBoundsDebugSub);
						rectShape.setOutlineThickness(-1);
						rectShape.setSize(Vector2f(circle.getRadius() * 2, circle.getRadius() * 2));
						rectShape.setPosition(circle.getPosition());
						globalTexture->draw(rectShape);
					}
					break;

				case Type::LINE:
					lineRect.setRotation(0);
					lineDeltaX = linePointBX.evaluate(thisSize.x) - linePointAX.evaluate(thisSize.x);
					lineDeltaY = linePointBY.evaluate(thisSize.y) - linePointAY.evaluate(thisSize.y);
					lineRect.setSize(Vector2f(sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY), lineThickness));
					lineRect.setOrigin(Vector2f(sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY) / 2, lineThickness / 2 + linePerpendicularOffset));
					if (acos(lineDeltaX / sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY)) <= PI / 2)
						lineRect.setRotation(asin(lineDeltaY / sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY)) / PI * 180.0);
					else
						lineRect.setRotation(180.0 - asin(lineDeltaY / sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY)) / PI * 180.0);
					lineRect.setPosition(
						thisPosition.x + linePointAX.evaluate(thisSize.x) + lineDeltaX / 2,
						thisPosition.y + linePointAY.evaluate(thisSize.y) + lineDeltaY / 2
					);
					lineRect.setFillColor(*lineFillColor);
					lineRect.setOutlineThickness(lineOutlineThickness);
					lineRect.setOutlineColor(*lineOutlineColor);
					globalTexture->draw(lineRect);

					if (resources.showDebugBoundaries) {
						RectangleShape rectShape;
						rectShape.setFillColor(*resources.colorTransparent);
						rectShape.setOutlineColor(*resources.colorUIBoundsDebugSub);
						rectShape.setOutlineThickness(-1);
						rectShape.setSize(Vector2f(lineRect.getGlobalBounds().width, lineRect.getGlobalBounds().height));
						rectShape.setPosition(Vector2f(lineRect.getGlobalBounds().left, lineRect.getGlobalBounds().top));
						globalTexture->draw(rectShape);
					}
					break;

				case Type::SIMPLE_TEXT:
					//cout << debugName << " rendering with font " << simpleTextFont << "\n";
					simpleText.setFont(*simpleTextFont);
					simpleText.setString(simpleTextString);
					simpleText.setCharacterSize(simpleTextSize);
					simpleText.setFillColor(*simpleTextColor);
					simpleText.setPosition(0, 0);
					simpleTextTempBoundsLeft = simpleText.getGlobalBounds().left;
					simpleTextTempBoundsTop = simpleText.getGlobalBounds().top;
					simpleText.setPosition(
						thisPosition.x + simpleTextAlignmentX * thisSize.x - simpleText.getGlobalBounds().left
						- simpleTextAlignmentX * simpleText.getGlobalBounds().width,
						thisPosition.y + simpleTextAlignmentY * thisSize.y
						- simpleTextAlignmentY *simpleText.getCharacterSize() - simpleText.getGlobalBounds().top
					);
					globalTexture->draw(simpleText);

					if (resources.showDebugBoundaries) {
						RectangleShape rectShape;
						rectShape.setFillColor(*resources.colorTransparent);
						rectShape.setOutlineColor(*resources.colorUIBoundsDebugSub);
						rectShape.setOutlineThickness(-1);
						rectShape.setSize(Vector2f(simpleText.getGlobalBounds().width, simpleText.getCharacterSize()));
						rectShape.setPosition(Vector2f(
							simpleText.getPosition().x + simpleTextTempBoundsLeft, 
							simpleText.getPosition().y + simpleTextTempBoundsTop
						));
						globalTexture->draw(rectShape);
					}
					break;

				}

				globalTexture->display();
			}

		};

		Body* body = new Body();

		struct State {
			bool mouseHovered = false;
			bool mouseHoveredThrough = false;
			bool mousePressedDown = false;
		};
		State state;

		Element* parent;
		deque<Element*> children;

		AdaptiveVector x;
		AdaptiveVector y;
		AdaptiveVector w;
		AdaptiveVector h;
		float originX;
		float originY;

		string debugName;

		enum class SizingMode { PER_AXIS, RELATIVE_TO_W, RELATIVE_TO_H };
		SizingMode sizingMode;

		struct Interaction {
			Element* mouseHoveredElement = nullptr;
		};

		Element(Element* _parent, string _debugName) {
			parent = _parent;
			if (_parent != nullptr)
				parent->children.push_back(this);
			x.set(0, 0);
			y.set(0, 0);
			w.set(1, 0);
			h.set(1, 0);
			originX = 0.0;
			originY = 0.0;
			sizingMode = SizingMode::PER_AXIS;
			debugName = _debugName;
			body->debugName = debugName;
		}

		Element(Element* _parent, string _debugName, AdaptiveVector _x, AdaptiveVector _y, AdaptiveVector _w, AdaptiveVector _h, float _originX, float _originY) {
			parent = _parent;
			if (_parent != nullptr)
				parent->children.push_back(this);
			x = _x;
			y = _y;
			w = _w;
			h = _h;
			originX = _originX;
			originY = _originY;
			sizingMode = SizingMode::PER_AXIS;
			debugName = _debugName;
			body->debugName = debugName;
		}

		~Element() {
			//cout << "deleted " << this->debugName << "\n";
			delete this->body;
			for (deque<Element*>::iterator child = this->children.begin(); child != this->children.end(); child++) {
				//cout << (*child)->debugName << "\n";
				delete (*child);
			}
				
		}

		void deleteElement() {
			if (this == nullptr)
				return;
			if (this->parent != nullptr)
				this->parent->children.erase(find(this->parent->children.begin(), this->parent->children.end(), this));
			delete this;
		}

		void linkContainer(Element* _parent) {
			//cout << this->debugName << " linked to " << _parent->debugName << "\n";
			if (this->parent == nullptr)
				eh.warn("relinking potential parent container, may lead to unwanted behavior!", __FILE__, __LINE__);
			if (this == _parent) {
				eh.warn("linking parent container to self, ignored, use delete instead", __FILE__, __LINE__);
				return;
			}
			if (this->parent != nullptr)
				this->parent->children.erase(find(this->parent->children.begin(), this->parent->children.end(), this));
			this->parent = _parent;
			this->parent->children.push_back(this);
		}

		void unlinkContainer() {
			if (this == this->parent) {
				eh.warn("root container can not be unlinked", __FILE__, __LINE__);
				return;
			}
			this->parent->children.erase(remove(this->parent->children.begin(), this->parent->children.end(), this), this->parent->children.end());
			this->parent = this;
		}

		Vector2u getTextureSize(Vector2u parentSize) {
			if (this->w.evaluate(parentSize.x) <= 0 || this->h.evaluate(parentSize.y) <= 0)
				eh.warn("texture size is smaller than 0, set to 1", __FILE__, __LINE__);
			return Vector2u(
				max((unsigned int)1, (unsigned int)this->w.evaluate(parentSize.x)),
				max((unsigned int)1, (unsigned int)this->h.evaluate(parentSize.y))
			);
		}

		
		void render(RenderTexture* globalTexture, Vector2f thisPosition, Vector2f thisSize, UI* uiEngine) {
			//cout << this->debugName << ": ";
			Resources resources;
			if (Mouse::getPosition(*uiEngine->window).x >= thisPosition.x &&
				Mouse::getPosition(*uiEngine->window).x <= thisPosition.x + thisSize.x &&
				Mouse::getPosition(*uiEngine->window).y >= thisPosition.y &&
				Mouse::getPosition(*uiEngine->window).y <= thisPosition.y + thisSize.y) {
				uiEngine->interaction->mouseHoveredElement = this;
				//cout << "mouse on " << this->debugName << "\n";
				this->state.mouseHovered = false;
				this->state.mouseHoveredThrough = true;
				//cout << "true\n";
			}
			else {
				this->state.mouseHoveredThrough = false;
				//cout << "false\n";
			}

			this->body->render(globalTexture, thisPosition, thisSize);

			for (deque<Element*>::iterator child = this->children.begin(); child != this->children.end(); child++) {
				Vector2f childSize((*child)->w.evaluate(thisSize.x), (*child)->h.evaluate(thisSize.y));
				//eh->info("child texture size: " + to_string(evaluatedW) + ", " + to_string(evaluatedH), __FILE__, __LINE__);

				if ((*child)->sizingMode == SizingMode::RELATIVE_TO_H)
					childSize.x = (*child)->w.evaluate(childSize.y);
				if ((*child)->sizingMode == SizingMode::RELATIVE_TO_W)
					childSize.y = (*child)->h.evaluate(childSize.x);
				
				// TODO

				(*child)->render(globalTexture, Vector2f(
					thisPosition.x + (*child)->x.evaluate(thisSize.x) - childSize.x * (*child)->originX,
					thisPosition.y + (*child)->y.evaluate(thisSize.y) - childSize.y * (*child)->originY
				), childSize, uiEngine);
	
			}
			globalTexture->display();
		}
	};




public:
	Resources* resources;
	Element* rootContainer;
	RenderWindow* window;
	Element::Interaction* interaction;

	UI(RenderWindow* _window, Resources* _resources) {
		window = _window;
		rootContainer = new Element(nullptr, "rootContainer");
		//cout << "root container set\n";
		rootContainer->x.set(0, window->getSize().x);
		rootContainer->y.set(0, window->getSize().y);
		interaction = new Element::Interaction;
		resources = _resources;
	}

	float framerate = 0.0;
	Clock framerateClock;
	Time previousFrame = framerateClock.getElapsedTime();

	void render() {
	
		framerate = (double)1000000 / (framerateClock.getElapsedTime() - previousFrame).asMicroseconds();
		//cout << framerate << "\n";
		previousFrame = framerateClock.getElapsedTime();

		Element* framerateDisplay = new Element(rootContainer, "framerateDisplay", { 0.0, 0 }, { 0.0, 0 }, { 0.0, 40 }, { 0.0, 20 }, 0.0, 0.0);
		
		framerateDisplay->body->setSimpleText(to_string((int)round(framerate)), resources->fontDefault, 20, resources->colorRed, 0.5, 0.5);

		window->clear(Color(0, 0, 0, 255));
		RenderTexture* windowTexture = new RenderTexture;
		if (!windowTexture->create(
			rootContainer->getTextureSize(window->getSize()).x, 
			rootContainer->getTextureSize(window->getSize()).y, 
			resources->contextSettings))
			eh.err("render texture failed to create", __FILE__, __LINE__);
		windowTexture->setSmooth(true);
		interaction->mouseHoveredElement = nullptr;

		rootContainer->render(windowTexture, Vector2f(0, 0), Vector2f(windowTexture->getSize().x, windowTexture->getSize().y), this);

		framerateDisplay->deleteElement();

		if (interaction->mouseHoveredElement != nullptr)
			interaction->mouseHoveredElement->state.mouseHovered = true;

		Sprite windowSprite;
		windowSprite.setTexture(windowTexture->getTexture(), true);
		windowSprite.setPosition(0, 0);

		window->draw(windowSprite);

		window->display();

		delete windowTexture;
	}

	void resetRootContainer() {
		delete rootContainer;
		rootContainer = new Element(nullptr, "rootContainer");
	}

};
