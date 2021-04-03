#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <set>
#include "ExceptionHandler.hpp"


using namespace std;
using namespace sf;

class Renderer {
private:
	ExceptionHandler* eh;

public:
	sf::Font font;
	sf::Font fontMono;

	struct AdaptiveVector {
		double relativeComponent;
		int absoluteComponent;

		AdaptiveVector(double _relativeComponent, int _absoluteComponent) {
			relativeComponent = _relativeComponent;
			absoluteComponent = _absoluteComponent;
		}

		void set(double _relativeComponent, int _absoluteComponent) {
			relativeComponent = _relativeComponent;
			absoluteComponent = _absoluteComponent;
		}

		double evaluate(double parent) {
			return parent * this->relativeComponent + this->absoluteComponent;
		}

	};
	/*
	class TextTexture {
	public:
		sf::Text* text = new Text();

		TextTexture(string _text, Renderer* renderer) {
			text.setFont(renderer->fontMono);
			text.setString(text);
			text.setCharacterSize(12);
			text.setFillColor(Color(50, 50, 50));
			text.setOrigin(Vector2f(text.getGlobalBounds().left, 12.0));
			//text.setPosition(Vector2f(Mouse::getPosition(*graphene->window).x, Mouse::getPosition(*graphene->window).y - 22));
			text.setPosition(Vector2f(10, graphene->window->getSize().y - 8.0));
			renderTexture->draw(text);
		}

		~TextTexture() {
			delete text;
		}

	};*/


	class UIElement {
	public:
		UIElement* parent;
		set<UIElement*> children;
		ExceptionHandler* eh = nullptr;
		AdaptiveVector* x;
		AdaptiveVector* y;
		AdaptiveVector* w;
		AdaptiveVector* h;
		AdaptiveVector* originX;
		AdaptiveVector* originY;
		enum class SizingMode { PER_AXIS, RATIO_BASED_ON_W, RATIO_BASED_ON_H };
		SizingMode sizingMode;

		bool useOverrideTexture = false;
		RenderTexture* overrideTexture = nullptr;

		UIElement(UIElement* _parent, ExceptionHandler* _eh) {
			eh = _eh;
			parent = _parent;
			x = new Renderer::AdaptiveVector(0, 0);
			y = new Renderer::AdaptiveVector(0, 0);
			w = new Renderer::AdaptiveVector(1, 0);
			h = new Renderer::AdaptiveVector(1, 0);
			originX = new Renderer::AdaptiveVector(0, 0);
			originY = new Renderer::AdaptiveVector(0, 0);
			sizingMode = SizingMode::PER_AXIS;
		}

		UIElement(RenderTexture* _overrideTexture, UIElement* _parent, ExceptionHandler* _eh) {
			eh = _eh;
			parent = _parent;
			x = new Renderer::AdaptiveVector(0, 0);
			y = new Renderer::AdaptiveVector(0, 0);
			w = new Renderer::AdaptiveVector(1, 0);
			h = new Renderer::AdaptiveVector(1, 0);
			originX = new Renderer::AdaptiveVector(0, 0);
			originY = new Renderer::AdaptiveVector(0, 0);
			sizingMode = SizingMode::PER_AXIS;

			useOverrideTexture = true;
			overrideTexture = _overrideTexture;
		}

		~UIElement() {
			delete x;
			delete y;
			delete w;
			delete h;
			delete originX;
			delete originY;

		}

		void unlinkContainer() {
			if (this == this->parent) {
				eh->warn("root container can not be unlinked", __LINE__);
				return;
			}
			this->parent->children.erase(this);
			this->parent = this;
		}

		Vector2u getTextureSize(Vector2u parentSize) {
			return Vector2u(this->w->evaluate(parentSize.x), this->h->evaluate(parentSize.y));
		}

		// parent provides the render texture for "this" windowElement
		void renderChildren(RenderTexture* thisTexture) {
			for (set<UIElement*>::iterator child = this->children.begin(); child != this->children.end(); child++) {
				RenderTexture* childTexture = new RenderTexture();
				if (!childTexture->create(
					(*child)->w->evaluate(thisTexture->getSize().x),
					(*child)->h->evaluate(thisTexture->getSize().y)))
					eh->err("render texture failed to create", __LINE__);
				if ((*child)->useOverrideTexture) {
					Sprite overrideTextureSprite;
					overrideTextureSprite.setTexture(overrideTexture->getTexture(), true);
					overrideTextureSprite.setPosition(0, 0);
					childTexture->draw(overrideTextureSprite);
				}
				(*child)->renderChildren(childTexture);
				Sprite childTextureSprite;
				childTextureSprite.setTexture(childTexture->getTexture(), true);
				childTextureSprite.setPosition(
					(*child)->x->evaluate(thisTexture->getSize().x) - (*child)->originX->evaluate(childTexture->getSize().x),
					(*child)->y->evaluate(thisTexture->getSize().y) - (*child)->originX->evaluate(childTexture->getSize().y)
				);
				thisTexture->draw(childTextureSprite);
				delete childTexture;
			}
			thisTexture->display();
		}
	};

	class Layout : public UIElement {
	public:
		void linkContainer(Layout* _parent) {
			if (this == this->parent) {
				eh->warn("root container can not be unlinked", __LINE__);
				return;
			}
			parent = _parent;
			_parent->children.insert(this);
		}

		
	};

	class SimpleText : public UIElement {
	public:
		UIElement* thisElement;
		Text* text;

		void linkContainer(Layout* _parent) {
			if (this == this->parent) {
				eh->warn("root container can not be unlinked", __LINE__);
				return;
			}
			parent = _parent;
			_parent->children.insert(this);
		}

		

		SimpleText(UIElement* _container, ExceptionHandler* _eh) : UIElement(_container, _eh) {
			thisElement = new UIElement(_container, _eh);
			text = new Text();
		}

		~SimpleText() {
			delete text;
		}

		void insert() {

		}

	};





	RenderWindow* window;
	UIElement* windowElement;

	Renderer(RenderWindow* _window, ExceptionHandler* _eh) {
		window = _window;
		windowElement = new UIElement(nullptr, _eh);
		windowElement->x->set(0, window->getSize().x);
		windowElement->y->set(0, window->getSize().y);

		eh = _eh;

		if (!font.loadFromFile(".\\Manrope-Medium.ttf"))
			eh->err("font Manrope-Medium.ttf failed to load", __LINE__);
		if (!fontMono.loadFromFile(".\\consola.ttf"))
			eh->err("font consola.ttf failed to load", __LINE__);

		UIElement* uiElement = new UIElement(windowElement, eh);
		SimpleText* simpleText = new SimpleText(uiElement, eh);

	}

	void render() {
		window->clear(Color(0, 0, 0, 255));

		RenderTexture* windowTexture = new RenderTexture;
		if (!windowTexture->create(windowElement->getTextureSize(window->getSize()).x, windowElement->getTextureSize(window->getSize()).y))
			eh->err("render texture failed to create", __LINE__);

		windowElement->renderChildren(windowTexture);
		Sprite windowSprite;
		windowSprite.setTexture(windowTexture->getTexture(), true);
		windowSprite.setPosition(0, 0);

		window->draw(windowSprite);

		window->display();
	}

};
