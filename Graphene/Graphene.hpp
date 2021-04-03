#pragma once
#define PI 3.14159265

#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "ExceptionHandler.hpp"


using namespace std;
using namespace sf;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(0.0, 1.0);

class Graphene {
public:
	RenderWindow* window;
	ExceptionHandler* eh;

	struct NormCoord {
		double x;
		double y;
		NormCoord(double _x, double _y) {
			x = _x;
			y = _y;
		}
	};

	struct Vertex {
		string name = "";
		bool nameVisible = false;
		string value = "";
		bool valueVisible = false;
		NormCoord coord = NormCoord(dis(gen), dis(gen));

		Vertex() {
		}
	};

	struct Edge {
		string name = "";
		bool nameVisible = false;
		string value = "";
		bool valueVisible = false;
		Vertex* startingVertex = nullptr;
		Vertex* endingVertex = nullptr;
		bool directed = false;

		Edge(Vertex* _startingVertex, Vertex* _endingVertex, bool _directed) {
			startingVertex = _startingVertex;
			endingVertex = _endingVertex;
			directed = _directed;

		}
	};

	vector<Vertex> verticies;
	vector<Edge> edges;

	// Handles all the graph rendering, scales automatically depending on the texture assigned.
	class Renderer {
	public:
		Graphene* graphene;
		RenderTexture* renderTexture;
		sf::Font font;
		sf::Font fontMono;
		ExceptionHandler* eh;


		Renderer(Graphene* _graphene, ExceptionHandler* _eh) {
			eh = _eh;
			graphene = _graphene;
			renderTexture = nullptr;
			eh->info("renderer rendertexture is not defined, use Graphene::Renderer::assignRenderTexture(sf::RenderTexture) to assign a texture for rendering", __LINE__);
			eh->info("the current rendering system is temporary", __LINE__);

			if (!font.loadFromFile(".\\Manrope-Medium.ttf"))
				eh->err("font Manrope-Medium.ttf failed to load", __LINE__);
			if (!fontMono.loadFromFile(".\\consola.ttf"))
				eh->err("font consola.ttf failed to load", __LINE__);

		}

		void assignRenderTexture(RenderTexture* _renderTexture) {
			renderTexture = _renderTexture;
		}

		Vector2f coordMapping(NormCoord coord) {
			if (coord.x < 0 || coord.x > 1)
				eh->warn("normalized coord x value is not in range 0.0 to 1.0", __LINE__);
			if (coord.y < 0 || coord.y > 1)
				eh->warn("normalized coord y value is not in range 0.0 to 1.0", __LINE__);

			double width = renderTexture->getSize().x;
			double height = renderTexture->getSize().y;
			double margin = 80;
			double mapSize = min(width - margin * 2, height - margin * 2);
			//eh->info("render texture size: " + to_string(width) + ", " + to_string(height), __LINE__);
			//eh->flushExceptionsToIOStream();
			return Vector2f((float)width / 2 - mapSize / 2 + coord.x * mapSize, (float)height / 2 - mapSize / 2 + coord.y * mapSize);
		}

		NormCoord coordMapping(Vector2f coord) {
			int margin = 80;
			int width = renderTexture->getSize().x;
			int height = renderTexture->getSize().y;
			int mapSize = min(width - margin * 2, height - margin * 2);

			return NormCoord((float)(coord.x - width / 2) / mapSize + 0.5, (float)(coord.y - height / 2) / mapSize + 0.5);
		}

		NormCoord coordMapping(Vector2i coord) {
			int margin = 80;
			int width = renderTexture->getSize().x;
			int height = renderTexture->getSize().y;
			int mapSize = min(width - margin * 2, height - margin * 2);

			return NormCoord((float)(coord.x - width / 2) / mapSize + 0.5, (float)(coord.y - height / 2) / mapSize + 0.5);
		}


		// this is a crude function to transform a rectangle shape into a line
		void lineShape(RectangleShape* rectShape, Vector2f pointA, Vector2f pointB, float width) {
			rectShape->setRotation(0);
			Vector2f delta;
			delta.x = pointB.x - pointA.x;
			delta.y = pointB.y - pointA.y;
			double diagonalDistance = sqrt(delta.x * delta.x + delta.y * delta.y);

			rectShape->setSize(Vector2f(diagonalDistance, width));
			rectShape->setOrigin(Vector2f(diagonalDistance / 2, width / 2));

			if (acos(delta.x / diagonalDistance) <= PI / 2)
				rectShape->setRotation(asin(delta.y / diagonalDistance) / PI * 180.0);
			else
				rectShape->setRotation(180.0 - asin(delta.y / diagonalDistance) / PI * 180.0);

			rectShape->setPosition(pointA.x + delta.x / 2, pointA.y + delta.y / 2);
		}

		void render() {
			float lineWidth = 5;

			eh->flushExceptionsToIOStream();

			for (vector<Edge>::iterator edge = graphene->edges.begin(); edge != graphene->edges.end(); edge++) {
				RectangleShape rectShape;
				rectShape.setFillColor(Color(120, 120, 120));
				//eh->flushExceptionsToIOStream();
				Vector2f startingPoint = coordMapping(edge->startingVertex->coord);
				Vector2f endingPoint = coordMapping(edge->endingVertex->coord);
				lineShape(&rectShape, startingPoint, endingPoint, lineWidth);

				renderTexture->draw(rectShape);
			}

			for (vector<Vertex>::iterator vertex = graphene->verticies.begin(); vertex != graphene->verticies.end(); vertex++) {
				Vector2f position = coordMapping(vertex->coord);

				CircleShape circleShape;
				circleShape.setFillColor(Color(224, 207, 52));
				circleShape.setRadius(20);
				circleShape.setOrigin(Vector2f(20, 20));
				circleShape.setPosition(position);
				circleShape.setOutlineColor(Color(201, 189, 60));
				circleShape.setOutlineThickness(5);
				renderTexture->draw(circleShape);

				sf::Text text;
				text.setFont(font);
				text.setString(vertex->name);
				text.setCharacterSize(30);
				text.setFillColor(Color(50, 50, 50));
				text.setOrigin(Vector2f(text.getGlobalBounds().left + text.getGlobalBounds().width / 2, 30));
				text.setPosition(Vector2f(position.x, position.y + 11));
				renderTexture->draw(text);

			}


			sf::Text text;
			text.setFont(fontMono);
			text.setString("(" + to_string(Mouse::getPosition(*graphene->window).x) + ", " + to_string(Mouse::getPosition(*graphene->window).y) + ")");
			text.setCharacterSize(12);
			text.setFillColor(Color(50, 50, 50));
			text.setOrigin(Vector2f(text.getGlobalBounds().left, 12.0));
			//text.setPosition(Vector2f(Mouse::getPosition(*graphene->window).x, Mouse::getPosition(*graphene->window).y - 22));
			text.setPosition(Vector2f(10, graphene->window->getSize().y - 8.0));
			renderTexture->draw(text);

			text.setString("(" + to_string(min(max(coordMapping(Mouse::getPosition(*graphene->window)).x, 0.0), 1.0)) + ", " + to_string(min(max(coordMapping(Mouse::getPosition(*graphene->window)).y, 0.0), 1.0)) + ")");
			text.setFillColor(Color(255, 0, 0));
			text.setOrigin(Vector2f(text.getGlobalBounds().left, 12.0));
			//text.setPosition(Vector2f(Mouse::getPosition(*graphene->window).x, Mouse::getPosition(*graphene->window).y - 6));
			text.setPosition(Vector2f(100, graphene->window->getSize().y - 8.0));
			renderTexture->draw(text);

		}

	};

	// Handles all the graph calculations
	class Core {
	public:
		Graphene* graphene;
		ExceptionHandler* eh;

		Core(Graphene* _graphene, ExceptionHandler* _eh) {
			graphene = _graphene;
			eh = _eh;
		}

	};

	// Handles the structure of a graph, and support for multiple graphs, graph stepping, highlighting, etc.
	class Graph {
	public:
		Graphene* graphene;
		ExceptionHandler* eh;

		Graph(Graphene* _graphene, ExceptionHandler* _eh) {
			graphene = _graphene;
			eh = _eh;
		}
	};


	Renderer* renderer;
	Core* core;
	Graph* graph;


	Graphene(RenderWindow* _window, ExceptionHandler* _eh) {
		renderer = new Renderer(this, _eh);
		core = new Core(this, _eh);
		graph = new Graph(this, _eh);
		window = _window;
		eh = _eh;
		eh->info("new graphene object declared", __LINE__);
		eh->flushExceptionsToIOStream();
	}

};
