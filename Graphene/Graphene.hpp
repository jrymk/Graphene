#pragma once
#define PI 3.14159265

#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "ExceptionHandler.hpp"
#include "UIEngine.hpp"

using namespace std;
using namespace sf;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(0.0, 1.0);

static class Graphene {
public:
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
		Resources resources;
		Graphene* graphene = nullptr;
		ExceptionHandler eh;
		UIEngine::UIElement* graphElement = nullptr;

		Renderer(Graphene* _graphene) {
			graphene = _graphene;
		}

		void updateGraphElement() {
			graphElement = new UIEngine::UIElement(nullptr, "graphElement");
			int edgeID = 0;
			for (vector<Edge>::iterator edge = graphene->edges.begin(); edge != graphene->edges.end(); edge++) {
				UIEngine::UIElement* edgeElement = new UIEngine::UIElement(graphElement, "edge" + to_string(edgeID));
				edgeElement->body->setLine(
					{ edge->startingVertex->coord.x, 0 }, { edge->startingVertex->coord.y, 0 },
					{ edge->endingVertex->coord.x, 0 }, { edge->endingVertex->coord.y, 0 },
					5, resources.colorGray);
			}
		}

	};

	// Handles all the graph calculations
	class Core {
	public:
		Graphene* graphene;

		Core(Graphene* _graphene) {
			graphene = _graphene;
		}

	};

	// Handles the structure of a graph, and support for multiple graphs, graph stepping, highlighting, etc.
	class Graph {
	public:
		Graphene* graphene;
		ExceptionHandler eh;

		Graph(Graphene* _graphene) {
			graphene = _graphene;
		}
	};


	Renderer* renderer;
	Core* core;
	Graph* graph;


	Graphene() {
		ExceptionHandler eh;
		renderer = new Renderer(this);
		core = new Core(this);
		graph = new Graph(this);
		eh.info("new graphene object declared", __FILE__, __LINE__);
	}

	~Graphene() {
		delete renderer;
		delete core;
		delete graph;
	}

};
