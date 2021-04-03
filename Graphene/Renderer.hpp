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

	struct Panel {
	public:
		Panel* parent;
		set<Panel*> children;
		ExceptionHandler* eh = nullptr;
		AdaptiveVector* x;
		AdaptiveVector* y;
		AdaptiveVector* w;
		AdaptiveVector* h;
		AdaptiveVector* originX;
		AdaptiveVector* originY;
		enum class SizingMode { PER_AXIS, RATIO_BASED_ON_W, RATIO_BASED_ON_H };
		SizingMode sizingMode;

	
		Panel(Vector2u size, Panel* _parent, ExceptionHandler* _eh) {
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

		~Panel() {

		}

		void linkParent(Panel* _parent) {
			parent = _parent;
			_parent->children.insert(this);

		}

		void unlinkParent() {
			this->parent->children.erase(this);
			this->parent = this;
		}

		Vector2u getTextureSize(Vector2u parentSize) {
			return Vector2u(this->w->evaluate(parentSize.x), this->h->evaluate(parentSize.y));
		}

		// parent provides the render texture for "this" windowPanel
		void render(RenderTexture* thisTexture) {
			for (set<Panel*>::iterator child = this->children.begin(); child != this->children.end(); child++) {
				RenderTexture* childTexture = new RenderTexture();
				if (!childTexture->create(
					(*child)->w->evaluate(thisTexture->getSize().x),
					(*child)->h->evaluate(thisTexture->getSize().y)))
					eh->err("render texture failed to create", __LINE__);
				(*child)->render(childTexture);
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

	RenderWindow* window;
	Panel* windowPanel;

	Renderer(RenderWindow* _window, ExceptionHandler* _eh) {
		window = _window;
		windowPanel = new Panel(_window->getSize(), nullptr, _eh);
		eh = _eh;
	}

	void render() {
		window->clear(Color(0, 0, 0, 255));

		RenderTexture* windowTexture = new RenderTexture;
		if (!windowTexture->create(windowPanel->getTextureSize(window->getSize()).x, windowPanel->getTextureSize(window->getSize()).y))
			eh->err("render texture failed to create", __LINE__);
		
		windowPanel->render(windowTexture);
		Sprite windowSprite;
		windowSprite.setTexture(windowTexture->getTexture(), true);
		windowSprite.setPosition(0, 0);

		window->draw(windowSprite);

		window->display();
	}


};
