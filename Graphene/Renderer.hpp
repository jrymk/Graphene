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

		double evaluate(unsigned int parent) {
			return (round(parent * this->relativeComponent + this->absoluteComponent));
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
			if (_parent != nullptr) {
				parent->children.insert(this);
				eh->ok("children added", __LINE__);
			}
			else eh->ok("root layout added", __LINE__);	
			
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
			if (_parent != nullptr) {
				parent->children.insert(this);
				eh->ok("children added", __LINE__);
			}
			else eh->ok("root layout added", __LINE__);

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
				eh->warn("root container can not be unlinked", __LINE__);
				return;
			}
			this->parent->children.erase(this);
			this->parent = this;
		}

		Vector2u getTextureSize(Vector2u parentSize) {
			return Vector2u(this->w->evaluate(parentSize.x), this->h->evaluate(parentSize.y));
		}

		

		// parent provides the render textTexture for "this" windowElement
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
				eh->warn("using override texture: feature is experimental", __LINE__);
				Sprite overrideTextureSprite;
				overrideTextureSprite.setTexture(overrideTexture->getTexture(), true);
				overrideTextureSprite.setPosition(
					this->originX->evaluate(thisTexture->getSize().x) - this->originX->evaluate(overrideTexture->getSize().x),
					this->originY->evaluate(thisTexture->getSize().y) - this->originY->evaluate(overrideTexture->getSize().y)
				);
				thisTexture->draw(overrideTextureSprite);
			}
			eh->info("my size: " + to_string(thisTexture->getSize().x) + ", " + to_string(thisTexture->getSize().y), __LINE__);
			eh->info("I have " + to_string(children.size()) + " kids.", __LINE__);
			for (set<UIElement*>::iterator child = this->children.begin(); child != this->children.end(); child++) {
				RenderTexture* childTexture = new RenderTexture();
				eh->info("child width rel " + to_string((*child)->w->relativeComponent), __LINE__);
				if (!childTexture->create(
					(*child)->w->evaluate(thisTexture->getSize().x),
					(*child)->h->evaluate(thisTexture->getSize().y)))
					eh->err("render texture failed to create", __LINE__);
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
			eh->ok("rendering text", __LINE__);
			/*text->setOrigin(Vector2f(
				text->getGlobalBounds().left + this->alignmentX->evaluate(text->getGlobalBounds().width),
				text->getCharacterSize()//this->alignmentY->evaluate(text->getCharacterSize() * 2)
			));*/
			/*text->setPosition(Vector2f(
				this->alignmentX->evaluate(thisTexture->getSize().x),
				this->alignmentY->evaluate(thisTexture->getSize().y)
			));*/
			//RenderTexture textTexture;
			if (!textTexture->create(
				text->getGlobalBounds().width,
				text->getCharacterSize() * 2)
				) eh->err("render texture failed to create", __LINE__);
			/*
			Sprite textTextureSprite;
			textTextureSprite.setTexture(textTexture.getTexture(), true);
			textTextureSprite.setPosition(0, 0);
			thisTexture->draw(*text);*/
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
			eh->err("render texture failed to create", __LINE__);

		windowElement->render(windowTexture);
		Sprite windowSprite;
		windowSprite.setTexture(windowTexture->getTexture(), true);
		windowSprite.setPosition(0, 0);

		window->draw(windowSprite);

		window->display();
		delete windowTexture;
	}

};
