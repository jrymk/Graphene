#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Resources.hpp"

using namespace std;

class UI2 {
public:
	class Renderer {
	public:
		vector<sf::Vertex> vertexArray;
		Resources resources; 

		struct Vertex {
			float _x;
			float _y;
			Color* _color;
			Vertex(float x, float y, Color* color) {
				_x = x;
				_y = y;
				_color = color;
			}
		};

		void appendLine(Vertex v0, Vertex v1) {
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v0._x, v0._y), *v0._color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v1._x, v1._y), *v1._color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v1._x, v1._y), *v1._color));
		}

		void appendTriangle(Vertex v0, Vertex v1, Vertex v2) {
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v0._x, v0._y), *v0._color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v1._x, v1._y), *v1._color));
			vertexArray.push_back(sf::Vertex(sf::Vector2f(v2._x, v2._y), *v2._color));
		}

		void appendQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3) {
			appendTriangle(v0, v1, v2);
			appendTriangle(v0, v2, v3);
		}


		void testRender(UI2* ui) {
			ui->_window->clear(Color(0, 0, 0, 255));
			vertexArray.clear();
			/*appendQuad({ 0, 0, resources.colorBackground }, { (float)window->getSize().x, 0, resources.colorBackground },
				{ (float)window->getSize().x, (float)window->getSize().y, resources.colorBackground }, { 0, (float)window->getSize().y, resources.colorBackground });
			*/
			appendTriangle({ 0, 0, resources.colorRed }, { 50, 100, resources.colorRed }, { 150, 300, resources.colorRed });

			ui->_window->draw(&vertexArray[0], vertexArray.size(), sf::Triangles);
			ui->_window->display();
		}


	};

	
	Renderer* _renderer;
	sf::RenderWindow* _window;

	UI2(sf::RenderWindow* window) {
		_window = window;
		_renderer = new Renderer();
	}

	void testRender() {
		_renderer->testRender(this);
	}

};