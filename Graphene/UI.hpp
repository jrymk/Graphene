#pragma once
#define PI (float)3.141592

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Resources.hpp"

uniform_real_distribution<> disNorm(0.0, 1.0);

using namespace std;

class UI2 {
public:
	sf::RenderWindow* _window;
	Resources* _resources;

	class Renderer {
	private:
		UI2* _ui;

	public:
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





		Renderer(UI2* ui) {
			_ui = ui;
		}

		void testRender(UI2* ui) {
			VertexArray vertexArray(ui);
			//ui->_window->clear(Color(0, 0, 0, 255));
			vertexArray.clear();

			vertexArray.appendRectangle({ 0, 0 }, { (float)ui->_window->getSize().x, (float)ui->_window->getSize().y }, ui->_resources->colorBackground);

			/*for (int i = 0; i < 140; i++) {
				for (int j = 0; j < 70; j++) {
					vertexArray.appendCircle({ (float)50 + i * 10, (float)50 + j * 10 }, 20, ui->_resources->colorTranslucentYellow, ui->_resources->circlePointCount);
				}
			}*/

			vertexArray.appendRoundedRectangle({ 50, 50 }, { 300, 300 }, 30, ui->_resources->colorPink, 20);


			vertexArray.appendCircle({ 960, 540 }, 300, ui->_resources->colorDeepSkyBlue, ui->_resources->circlePointCount);
			vertexArray.appendLine({ 960, 540 }, { (float)sf::Mouse::getPosition(*ui->_window).x, (float)sf::Mouse::getPosition(*ui->_window).y }, 100, 100, ui->_resources->colorOrange);

			cout << "triangles rendered: " << vertexArray.getSize() << "\n";

			ui->_window->draw(vertexArray.getBuffer(), vertexArray.getSize(), sf::Triangles);
			ui->_window->display();
		}

	};

	Renderer* _renderer;

	/*class Element {
		class Body {
		public:
			UI2* _ui;
			enum class Type {
				NONE,
				CIRCLE,
				LINE,
				SIMPLE_TEXT,

			};
			Type _type = Type::NONE;
			Color* _backgroundColor = _ui->_resources->colorTransparent;
			string _debugName;

			// none


			// circle
			CircleShape circle;
			AdaptiveVector circleRadius = { 1.0, 0 };
			Color* circleFillColor = resources.colorTransparent;
			float circleOutlineThickness = 0;
			Color* circleOutlineColor = resources.colorTransparent;

			// rectangle


			// line
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

		};

	};


	UI2(sf::RenderWindow* window, Resources* resources) {
		_window = window;
		_renderer = new Renderer(this);
		_resources = resources;
	}*/


};