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

class UIEngine {
private:
	ExceptionHandler eh;

public:
	struct AdaptiveVector {
		double relativeComponent;
		int absoluteComponent;

		AdaptiveVector() {
			relativeComponent = 0.0;
			absoluteComponent = 0;
		}

		AdaptiveVector(double _relativeComponent, int _absoluteComponent) {
			relativeComponent = _relativeComponent;
			absoluteComponent = _absoluteComponent;
		}

		void set(double _relativeComponent, int _absoluteComponent) {
			relativeComponent = _relativeComponent;
			absoluteComponent = _absoluteComponent;
		}

		void fillParent() {
			relativeComponent = 1.0;
			absoluteComponent = 0;
		}

		void parentRatio(double ratio) {
			relativeComponent = ratio;
			absoluteComponent = 0;
		}

		void absolute(double value) {
			relativeComponent = 0.0;
			absoluteComponent = value;
		}

		void reverseAbsolute(double value) {
			relativeComponent = 1.0;
			absoluteComponent = value;
		}

		double evaluate(unsigned int parent) {
			return (round(parent * this->relativeComponent + this->absoluteComponent));
		}

	};

	class UIElement {
	private:
		ExceptionHandler eh;
		
	public:

		struct Body {
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

			// none


			// circle
			CircleShape circle;
			AdaptiveVector circleRadius = {1.0, 0};
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
			float lineOutlineThickness = 0;
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
				lineOutlineThickness = 0;
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

			void render(RenderTexture* texture) {
				RectangleShape rectShape;
				if (resources.showDebugBoundaries) {
					rectShape.setOutlineColor(*resources.colorUIBoundsDebug);
					rectShape.setOutlineThickness(1);
					rectShape.setSize(Vector2f(texture->getSize().x - 2, texture->getSize().y - 2));
					rectShape.setPosition(Vector2f(1, 1));
				}
				else {
					rectShape.setSize(Vector2f(texture->getSize().x, texture->getSize().y));
					rectShape.setPosition(Vector2f(0, 0));
				}
				rectShape.setFillColor(*(this->backgroundColor));
				texture->draw(rectShape);

				switch (this->type) {
				case Type::NONE:
					break;

				case Type::CIRCLE:
					circle.setRadius(circleRadius.evaluate(min(texture->getSize().x, texture->getSize().y)));
					circle.setFillColor(*circleFillColor);
					circle.setOutlineThickness(circleOutlineThickness);
					circle.setOutlineColor(*circleOutlineColor);
					circle.setPosition(Vector2f(
						AdaptiveVector(0.5, 0).evaluate(texture->getSize().x) - circle.getRadius(),
						AdaptiveVector(0.5, 0).evaluate(texture->getSize().y) - circle.getRadius()
					));
					texture->draw(circle);

					if (resources.showDebugBoundaries) {
						RectangleShape rectShape;
						rectShape.setFillColor(*resources.colorTransparent);
						rectShape.setOutlineColor(*resources.colorUIBoundsDebugSub);
						rectShape.setOutlineThickness(-1);
						rectShape.setSize(Vector2f(circle.getRadius() * 2, circle.getRadius() * 2));
						rectShape.setPosition(circle.getPosition());
						texture->draw(rectShape);
					}
					break;

				case Type::LINE:
					lineRect.setRotation(0);
					lineDeltaX = linePointBX.evaluate(texture->getSize().x) - linePointAX.evaluate(texture->getSize().x);
					lineDeltaY = linePointBY.evaluate(texture->getSize().y) - linePointAY.evaluate(texture->getSize().y);
					lineRect.setSize(Vector2f(sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY), lineThickness));
					lineRect.setOrigin(Vector2f(sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY) / 2, lineThickness / 2 + linePerpendicularOffset));
					if (acos(lineDeltaX / sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY)) <= PI / 2)
						lineRect.setRotation(asin(lineDeltaY / sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY)) / PI * 180.0);
					else
						lineRect.setRotation(180.0 - asin(lineDeltaY / sqrt(lineDeltaX * lineDeltaX + lineDeltaY * lineDeltaY)) / PI * 180.0);
					lineRect.setPosition(linePointAX.evaluate(texture->getSize().x) + lineDeltaX / 2, linePointAY.evaluate(texture->getSize().y) + lineDeltaY / 2);
					lineRect.setFillColor(*lineFillColor);
					lineRect.setOutlineThickness(lineOutlineThickness);
					lineRect.setOutlineColor(*lineOutlineColor);
					texture->draw(lineRect);

					if (resources.showDebugBoundaries) {
						RectangleShape rectShape;
						rectShape.setFillColor(*resources.colorTransparent);
						rectShape.setOutlineColor(*resources.colorUIBoundsDebugSub);
						rectShape.setOutlineThickness(-1);
						rectShape.setSize(Vector2f(lineRect.getSize().x, lineRect.getSize().y));
						rectShape.setPosition(Vector2f(lineRect.getPosition().x, lineRect.getPosition().y));
						texture->draw(rectShape);
					}
					break;

				case Type::SIMPLE_TEXT:
					simpleText.setFont(*simpleTextFont);
					simpleText.setString(simpleTextString);
					simpleText.setCharacterSize(simpleTextSize);
					simpleText.setFillColor(*simpleTextColor);
					simpleText.setPosition(Vector2f(0, 0));
					simpleTextTempBoundsLeft = simpleText.getGlobalBounds().left;
					simpleText.setPosition(
						AdaptiveVector(simpleTextAlignmentX, 0).evaluate(texture->getSize().x) - simpleText.getGlobalBounds().left
						- AdaptiveVector(simpleTextAlignmentX, 0).evaluate(simpleText.getGlobalBounds().width),
						AdaptiveVector(simpleTextAlignmentY, 0).evaluate(texture->getSize().y)
						- AdaptiveVector(simpleTextAlignmentY, 0).evaluate(simpleText.getCharacterSize() * 2)
					);
					texture->draw(simpleText);

					if (resources.showDebugBoundaries) {
						RectangleShape rectShape;
						rectShape.setFillColor(*resources.colorTransparent);
						rectShape.setOutlineColor(*resources.colorUIBoundsDebugSub);
						rectShape.setOutlineThickness(-1);
						rectShape.setSize(Vector2f(simpleText.getGlobalBounds().width, simpleText.getCharacterSize() * 2));
						rectShape.setPosition(Vector2f(simpleText.getPosition().x + simpleTextTempBoundsLeft, simpleText.getPosition().y));
						texture->draw(rectShape);
					}
					break;

				}

				texture->display();
			}

		};

		Body* body = nullptr;

		struct State {
			bool mouseHovered = false;
			bool mouseHoveredThrough = false;
			bool mousePressedDown = false;
		};
		State state;

		UIElement* parent;
		vector<UIElement*> children;

		AdaptiveVector x;
		AdaptiveVector y;
		AdaptiveVector w;
		AdaptiveVector h;
		AdaptiveVector originX;
		AdaptiveVector originY;

		string debugName;

		enum class SizingMode { PER_AXIS, RELATIVE_TO_W, RELATIVE_TO_H };
		SizingMode sizingMode;

		struct Interaction {
			UIElement* mouseHoveredElement = nullptr;
		};

		UIElement(UIElement* _parent, string _debugName) {
			parent = _parent;
			body = new Body();
			if (_parent != nullptr)
				parent->children.push_back(this);
			x.set(0, 0);
			y.set(0, 0);
			w.set(1, 0);
			h.set(1, 0);
			originX.set(0, 0);
			originY.set(0, 0);
			sizingMode = SizingMode::PER_AXIS;
			debugName = _debugName;
		}

		UIElement(UIElement* _parent, string _debugName, AdaptiveVector _x, AdaptiveVector _y, AdaptiveVector _w, AdaptiveVector _h, AdaptiveVector _originX, AdaptiveVector _originY) {
			parent = _parent;
			body = new Body();
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
		}

		~UIElement() {
			delete body;
			for (vector<UIElement*>::iterator child = this->children.begin(); child != this->children.end(); child++)
				delete (*child);
		}

		void linkContainer(UIElement* _parent) {
			if (this->parent == nullptr)
				eh.warn("relinking potential parent container, may lead to unwanted behavior!", __FILE__, __LINE__);
			if (this == _parent) {
				eh.warn("linking parent container to self, ignored, use delete instead", __FILE__, __LINE__);
				return;
			}
			if (this->parent != nullptr)
				this->parent->children.erase(remove(this->parent->children.begin(), this->parent->children.end(), this), this->parent->children.end());
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

		// parent provides the render texture for "this" rootContainer
		void render(RenderTexture* thisTexture, Vector2i globalPosition, UIEngine* uiEngine) {
			Resources resources;
			if (Mouse::getPosition(*uiEngine->window).x >= globalPosition.x && Mouse::getPosition(*uiEngine->window).x <= globalPosition.x + thisTexture->getSize().x &&
				Mouse::getPosition(*uiEngine->window).y >= globalPosition.y && Mouse::getPosition(*uiEngine->window).y <= globalPosition.y + thisTexture->getSize().y) {
				uiEngine->interaction->mouseHoveredElement = this;
				this->state.mouseHovered = false;
				this->state.mouseHoveredThrough = true;
			}

			thisTexture->clear(Color(0, 0, 0, 0));

			this->body->render(thisTexture);

			for (vector<UIElement*>::iterator child = this->children.begin(); child != this->children.end(); child++) {
				RenderTexture* childTexture = new RenderTexture();

				unsigned int evaluatedW = (*child)->w.evaluate(thisTexture->getSize().x);
				unsigned int evaluatedH = (*child)->h.evaluate(thisTexture->getSize().y);
				//eh->info("child texture size: " + to_string(evaluatedW) + ", " + to_string(evaluatedH), __FILE__, __LINE__);

				if ((*child)->sizingMode == SizingMode::RELATIVE_TO_H)
					evaluatedW = (*child)->w.evaluate(evaluatedH);
				if ((*child)->sizingMode == SizingMode::RELATIVE_TO_W)
					evaluatedH = (*child)->h.evaluate(evaluatedW);

				if (!childTexture->create(evaluatedW, evaluatedH, resources.contextSettings))
					eh.err("render texture failed to create", __FILE__, __LINE__);
				(*child)->render(childTexture, Vector2i(
					globalPosition.x + (*child)->x.evaluate(thisTexture->getSize().x) - (*child)->originX.evaluate(childTexture->getSize().x),
					globalPosition.y + (*child)->y.evaluate(thisTexture->getSize().y) - (*child)->originY.evaluate(childTexture->getSize().y)
				), uiEngine);
				Sprite childTextureSprite;
				childTextureSprite.setTexture(childTexture->getTexture(), true);
				childTextureSprite.setPosition(
					(*child)->x.evaluate(thisTexture->getSize().x) - (*child)->originX.evaluate(childTexture->getSize().x),
					(*child)->y.evaluate(thisTexture->getSize().y) - (*child)->originY.evaluate(childTexture->getSize().y)
				);
				thisTexture->draw(childTextureSprite);
				delete childTexture;
			}
			thisTexture->display();
		}
	};




public:
	UIElement* rootContainer;
	RenderWindow* window;
	UIElement::Interaction* interaction;

	UIEngine(RenderWindow* _window) {
		window = _window;
		rootContainer = new UIElement(nullptr, "rootContainer");
		cout << "root container set\n";
		rootContainer->x.set(0, window->getSize().x);
		rootContainer->y.set(0, window->getSize().y);
		interaction = new UIElement::Interaction;
	}

	void render() {
		Resources resources;
		window->clear(Color(0, 0, 0, 255));
		RenderTexture* windowTexture = new RenderTexture;
		if (!windowTexture->create(rootContainer->getTextureSize(window->getSize()).x, rootContainer->getTextureSize(window->getSize()).y, resources.contextSettings))
			eh.err("render texture failed to create", __FILE__, __LINE__);
		windowTexture->setSmooth(true);
		interaction->mouseHoveredElement = nullptr;

		rootContainer->render(windowTexture, Vector2i(0, 0), this);

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
		rootContainer = new UIElement(nullptr, "rootContainer");
	}

};
