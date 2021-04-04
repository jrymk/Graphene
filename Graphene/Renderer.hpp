#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <set>
#include "ExceptionHandler.hpp"
#include "Resources.hpp"
#include <random>

using namespace std;
using namespace sf;

uniform_int_distribution<> dis255(0, 255);

class Renderer {
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
		UIElement* parent;
		set<UIElement*> children;

		AdaptiveVector x;
		AdaptiveVector y;
		AdaptiveVector w;
		AdaptiveVector h;
		AdaptiveVector originX;
		AdaptiveVector originY;

		enum class SizingMode { PER_AXIS, RELATIVE_TO_W, RELATIVE_TO_H };
		SizingMode sizingMode;

		Color backgroundColor;

		bool useOverrideTexture = false;
		RenderTexture* overrideTexture = nullptr;
		AdaptiveVector alignmentX;
		AdaptiveVector alignmentY;

		UIElement(UIElement* _parent) {
			parent = _parent;
			if (_parent != nullptr)
				parent->children.insert(this);
			x.set(0, 0);
			y.set(0, 0);
			w.set(1, 0);
			h.set(1, 0);
			originX.set(0, 0);
			originY.set(0, 0);
			alignmentX.set(0.5, 0);
			sizingMode = SizingMode::PER_AXIS;

			alignmentX.set(0.5, 0);
			alignmentY.set(0.5, 0);

			backgroundColor = Color(0, 0, 0, 0);
		}

		UIElement(UIElement* _parent, AdaptiveVector _x, AdaptiveVector _y, AdaptiveVector _w, AdaptiveVector _h, AdaptiveVector _originX, AdaptiveVector _originY) {
			parent = _parent;
			if (_parent != nullptr)
				parent->children.insert(this);
			x = _x;
			y = _y;
			w = _w;
			h = _h;
			originX = _originX;
			originY = _originY;
			sizingMode = SizingMode::PER_AXIS;

			alignmentX.set(0.5, 0);
			alignmentY.set(0.5, 0);

			backgroundColor = Color(0, 0, 0, 0);
		}

		UIElement(RenderTexture* _overrideTexture, UIElement* _parent) {
			parent = _parent;
			if (_parent != nullptr)
				parent->children.insert(this);
			x.set(0.0, 0);
			y.set(0.0, 0);
			w.set(1.0, 0);
			h.set(1.0, 0);
			originX.set(0.0, 0);
			originY.set(0.0, 0);
			sizingMode = SizingMode::PER_AXIS;

			useOverrideTexture = true;
			overrideTexture = _overrideTexture;
			alignmentX.set(0.5, 0);
			alignmentY.set(0.5, 0);

			backgroundColor = Color(0, 0, 0, 0);
		}

		UIElement(RenderTexture* _overrideTexture, UIElement* _parent, AdaptiveVector _alignmentX, AdaptiveVector _alignmentY) {
			parent = _parent;
			if (_parent != nullptr)
				parent->children.insert(this);
			x.set(0.0, 0);
			y.set(0.0, 0);
			w.set(1.0, 0);
			h.set(1.0, 0);
			originX.set(0, 0);
			originY.set(0, 0);
			sizingMode = SizingMode::PER_AXIS;

			useOverrideTexture = true;
			overrideTexture = _overrideTexture;
			alignmentX = _alignmentX;
			alignmentY = _alignmentY;

			backgroundColor = Color(0, 0, 0, 0);
		}

		UIElement(RenderTexture* _overrideTexture, UIElement* _parent, 
			AdaptiveVector _x, AdaptiveVector _y, AdaptiveVector _w, AdaptiveVector _h, AdaptiveVector _originX, AdaptiveVector _originY, AdaptiveVector _alignmentX, AdaptiveVector _alignmentY) {
			parent = _parent;
			if (_parent != nullptr)
				parent->children.insert(this);
			x = _x;
			y = _y;
			w = _w;
			h = _h;
			originX = _originX;
			originY = _originY;
			sizingMode = SizingMode::PER_AXIS;

			useOverrideTexture = true;
			overrideTexture = _overrideTexture;
			alignmentX = _alignmentX;
			alignmentY = _alignmentY;

			backgroundColor = Color(0, 0, 0, 0);
		}

		~UIElement() {

		}

		void linkContainer(UIElement* _parent) {
			if (this == this->parent)
				eh.warn("relinking potential parent container, may lead to unwanted behavior!", __FILE__, __LINE__);
			if (this == _parent) {
				eh.warn("linking parent container to self, ignored, use delete instead", __FILE__, __LINE__);
				return;
			}
			this->parent->children.erase(this);
			this->parent = this;
		}

		void unlinkContainer() {
			if (this == this->parent) {
				eh.warn("root container can not be unlinked", __FILE__, __LINE__);
				return;
			}
			this->parent->children.erase(this);
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

		// parent provides the render texture for "this" windowElement
		void render(RenderTexture* thisTexture) {
			thisTexture->clear(Color(0, 0, 0, 0));
			RectangleShape rectShape;
			rectShape.setOutlineColor(Color(255, 180, 0, 255));
			rectShape.setOutlineThickness(1);
			rectShape.setSize(Vector2f(thisTexture->getSize().x - 2, thisTexture->getSize().y - 2));
			rectShape.setPosition(Vector2f(1, 1));
			rectShape.setFillColor(this->backgroundColor);
			thisTexture->draw(rectShape);

			if (this->useOverrideTexture) {
				Sprite overrideTextureSprite;
				overrideTextureSprite.setTexture(overrideTexture->getTexture(), true);
				overrideTextureSprite.setPosition(
					this->alignmentX.evaluate(thisTexture->getSize().x) - this->alignmentX.evaluate(overrideTexture->getSize().x),
					this->alignmentY.evaluate(thisTexture->getSize().y) - this->alignmentY.evaluate(overrideTexture->getSize().y)
				);
				thisTexture->draw(overrideTextureSprite);

				rectShape.setSize(Vector2f(overrideTexture->getSize().x - 2, overrideTexture->getSize().y - 2));
				rectShape.setPosition(Vector2f(
					this->alignmentX.evaluate(thisTexture->getSize().x) - this->alignmentX.evaluate(overrideTexture->getSize().x), 
					this->alignmentY.evaluate(thisTexture->getSize().y) - this->alignmentY.evaluate(overrideTexture->getSize().y)
				));
				rectShape.setOutlineColor(Color(180, 255, 0, 255));
				thisTexture->draw(rectShape);
			}

			for (set<UIElement*>::iterator child = this->children.begin(); child != this->children.end(); child++) {
				RenderTexture* childTexture = new RenderTexture();

				unsigned int evaluatedW = (*child)->w.evaluate(thisTexture->getSize().x);
				unsigned int evaluatedH = (*child)->h.evaluate(thisTexture->getSize().y);
				//eh->info("child texture size: " + to_string(evaluatedW) + ", " + to_string(evaluatedH), __FILE__, __LINE__);

				if ((*child)->sizingMode == SizingMode::RELATIVE_TO_H)
					evaluatedW = (*child)->w.evaluate(evaluatedH);
				if ((*child)->sizingMode == SizingMode::RELATIVE_TO_W)
					evaluatedH = (*child)->h.evaluate(evaluatedW);

				if (!childTexture->create(evaluatedW, evaluatedH))
					eh.err("render texture failed to create", __FILE__, __LINE__);
				(*child)->render(childTexture);
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

	class SimpleText {
	private:
		ExceptionHandler eh;
		RenderTexture texture;
		Text text;

	public:
		SimpleText() {

		}

		SimpleText(string str, Font* font, int size, Color* color) {
			text.setFont(*font);
			text.setString(str);
			text.setCharacterSize(size);
			text.setFillColor(*color);
		}

		~SimpleText() {

		}

		RenderTexture* getTexture() {
			if (!texture.create(
				text.getGlobalBounds().width,
				text.getCharacterSize() * 2)
				) eh.err("render texture failed to create", __FILE__, __LINE__);
			texture.draw(text);
			texture.display();
			return &texture;
		}
	};

private:
	RenderWindow* window;

public:
	UIElement* windowElement;

	Renderer(RenderWindow* _window) {
		window = _window;
		windowElement = new UIElement(nullptr);
		windowElement->x.set(0, window->getSize().x);
		windowElement->y.set(0, window->getSize().y);
	}

	void render() {
		window->clear(Color(0, 0, 0, 255));

		RenderTexture* windowTexture = new RenderTexture;
		if (!windowTexture->create(windowElement->getTextureSize(window->getSize()).x, windowElement->getTextureSize(window->getSize()).y))
			eh.err("render texture failed to create", __FILE__, __LINE__);

		windowElement->render(windowTexture);
		Sprite windowSprite;
		windowSprite.setTexture(windowTexture->getTexture(), true);
		windowSprite.setPosition(0, 0);

		window->draw(windowSprite);

		window->display();
		delete windowTexture;
	}

};
