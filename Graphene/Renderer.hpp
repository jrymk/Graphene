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
	ExceptionHandler* eh;

public:
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

		enum class SizingMode { PER_AXIS, RELATIVE_TO_W, RELATIVE_TO_H };
		SizingMode sizingMode;

		bool useOverrideTexture = false;
		RenderTexture* overrideTexture = nullptr;

		UIElement(UIElement* _parent, ExceptionHandler* _eh) {
			eh = _eh;
			parent = _parent;
			if (_parent != nullptr) 
				parent->children.insert(this);
			
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
			if (_parent != nullptr) 
				parent->children.insert(this);

			x = new Renderer::AdaptiveVector(0.5, 0);
			y = new Renderer::AdaptiveVector(0.5, 0);
			w = new Renderer::AdaptiveVector(0, _overrideTexture->getSize().x);
			h = new Renderer::AdaptiveVector(0, _overrideTexture->getSize().y);
			originX = new Renderer::AdaptiveVector(0.5, 0);
			originY = new Renderer::AdaptiveVector(0.5, 0);
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
				eh->warn("root container can not be unlinked", __FILE__, __LINE__);
				return;
			}
			this->parent->children.erase(this);
			this->parent = this;
		}

		Vector2u getTextureSize(Vector2u parentSize) {
			if (this->w->evaluate(parentSize.x) <= 0 || this->h->evaluate(parentSize.y) <= 0)
				eh->warn("texture size is smaller than 0, set to 1", __FILE__, __LINE__);
			return Vector2u(
				max((unsigned int)1, (unsigned int)this->w->evaluate(parentSize.x)), 
				max((unsigned int)1, (unsigned int)this->h->evaluate(parentSize.y))
			);
		}

		

		// parent provides the render texture for "this" windowElement
		void render(RenderTexture* thisTexture) {
			thisTexture->clear(Color(0, 0, 0, 0));
			RectangleShape rectShape;
			rectShape.setOutlineColor(Color(255, 200, 0, 255));
			rectShape.setOutlineThickness(3);
			rectShape.setSize(Vector2f(thisTexture->getSize().x - 6, thisTexture->getSize().y - 6));
			rectShape.setPosition(Vector2f(3, 3));
			rectShape.setFillColor(Color(0, 0, 0, 0));
			thisTexture->draw(rectShape);

			if (this->useOverrideTexture) {
				Sprite overrideTextureSprite;
				overrideTextureSprite.setTexture(overrideTexture->getTexture(), true);
				overrideTextureSprite.setPosition(
					this->originX->evaluate(thisTexture->getSize().x) - this->originX->evaluate(overrideTexture->getSize().x),
					this->originY->evaluate(thisTexture->getSize().y) - this->originY->evaluate(overrideTexture->getSize().y)
				);
				thisTexture->draw(overrideTextureSprite);
			}

			for (set<UIElement*>::iterator child = this->children.begin(); child != this->children.end(); child++) {
				RenderTexture* childTexture = new RenderTexture();
			
				unsigned int evaluatedW = (*child)->w->evaluate(thisTexture->getSize().x);
				unsigned int evaluatedH = (*child)->h->evaluate(thisTexture->getSize().y);
				//eh->info("child texture size: " + to_string(evaluatedW) + ", " + to_string(evaluatedH), __FILE__, __LINE__);

				if ((*child)->sizingMode == SizingMode::RELATIVE_TO_H)
					evaluatedW = (*child)->w->evaluate(evaluatedH);
				if ((*child)->sizingMode == SizingMode::RELATIVE_TO_W)
					evaluatedH = (*child)->h->evaluate(evaluatedW);
				
				if (!childTexture->create(evaluatedW, evaluatedH))
					eh->err("render texture failed to create", __FILE__, __LINE__);
				(*child)->render(childTexture);
				Sprite childTextureSprite;
				childTextureSprite.setTexture(childTexture->getTexture(), true);
				childTextureSprite.setPosition(
					(*child)->x->evaluate(thisTexture->getSize().x) - (*child)->originX->evaluate(childTexture->getSize().x),
					(*child)->y->evaluate(thisTexture->getSize().y) - (*child)->originY->evaluate(childTexture->getSize().y)
				);
				thisTexture->draw(childTextureSprite);
				delete childTexture;
			}
			thisTexture->display();
		}
	};

	class SimpleText {
	public:
		ExceptionHandler* eh;
		Text* text;
		RenderTexture* textTexture = new RenderTexture();

		SimpleText(ExceptionHandler* _eh) {
			eh = _eh;
			text = new Text();
		}

		SimpleText(string str, Font* font, unsigned int size, Color* color, ExceptionHandler* _eh) {
			eh = _eh;
			text = new Text();
			text->setFont(*font);
			text->setString(str);
			text->setCharacterSize(size);
			text->setFillColor(*color);
		}

		~SimpleText() {
			delete text;
			delete textTexture;
		}

		void setText(string str, Font* font, unsigned int size, Color* color) {
			text->setFont(*font);
			text->setString(str);
			text->setCharacterSize(size);
			text->setFillColor(*color);
		}

		RenderTexture* getTexture() {
			if (!textTexture->create(
				text->getGlobalBounds().width,
				text->getCharacterSize() * 2)
				) eh->err("render texture failed to create", __FILE__, __LINE__);
			textTexture->draw(*text);
			textTexture->display();
			return textTexture;
		}

	};





	RenderWindow* window;
	UIElement* windowElement;
	vector<Font> font;

	Renderer(RenderWindow* _window, ExceptionHandler* _eh){
		eh = _eh;
		window = _window;
		windowElement = new UIElement(nullptr, eh);
		windowElement->x->set(0, window->getSize().x);
		windowElement->y->set(0, window->getSize().y);

	}

	void render() {
		window->clear(Color(255, 255, 255, 255));

		RenderTexture* windowTexture = new RenderTexture;
		if (!windowTexture->create(windowElement->getTextureSize(window->getSize()).x, windowElement->getTextureSize(window->getSize()).y))
			eh->err("render texture failed to create", __FILE__, __LINE__);

		windowElement->render(windowTexture);
		Sprite windowSprite;
		windowSprite.setTexture(windowTexture->getTexture(), true);
		windowSprite.setPosition(0, 0);

		window->draw(windowSprite);

		window->display();
		delete windowTexture;
	}

};
