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
		Graphene* graphene = nullptr;
		ExceptionHandler eh;
		UI::Element* graphElement = nullptr;
		Resources* resources = nullptr;

		Renderer(Graphene* _graphene) {
			graphene = _graphene;
			resources = _graphene->resources;
		}

		void newGraphElement() {
			graphElement = new UI::Element(nullptr, "graphElement");
			int edgeID = 0;
			for (vector<Edge>::iterator edge = graphene->edges.begin(); edge != graphene->edges.end(); edge++) {
				UI::Element* edgeElement = new UI::Element(graphElement, "edge" + to_string(edgeID));
				edgeElement->body->setLine(
					{ (float)edge->startingVertex->coord.x, 0 }, { (float)edge->startingVertex->coord.y, 0 },
					{ (float)edge->endingVertex->coord.x, 0 }, { (float)edge->endingVertex->coord.y, 0 },
					5, resources->colorGray);
				edgeID++;
			}
			int vertexID = 0;
			for (vector<Vertex>::iterator vertex = graphene->verticies.begin(); vertex != graphene->verticies.end(); vertex++) {
				UI::Element* vertexElement = new UI::Element(graphElement, "vertex" + to_string(edgeID),
					{ (float)vertex->coord.x, 0 }, { (float)vertex->coord.y, 0 }, { 0, 60 }, { 0, 60 }, 0.5, 0.5);
				vertexElement->body->setCircle({ 0.0, 20 }, resources->colorGold, -4, resources->colorGoldenrod);
				UI::Element* vertexTextElement = new UI::Element(vertexElement, "vertexText" + to_string(edgeID));
				vertexTextElement->body->setSimpleText(vertex->name, resources->fontDefault, 30, resources->colorText, 0.5, 0.63);
				vertexID++;
			}


		}

	};

	// Handles all the graph calculations
	class Core {
	public:
		Graphene* graphene;
		Resources* resources;

		Core(Graphene* _graphene) {
			graphene = _graphene;
			resources = _graphene->resources;
		}

		void init() {

		}

		void update() {

		}

	};

	// Handles the structure of a graph, and support for multiple graphs, graph stepping, highlighting, etc.
	class Graph {
	public:
		Graphene* graphene;
		ExceptionHandler eh;
		Resources* resources;

		Graph(Graphene* _graphene) {
			graphene = _graphene;
			resources = _graphene->resources;
		}
	};


	Resources* resources;
	Renderer* renderer;
	Core* core;
	Graph* graph;


	Graphene(Resources* _resources) {
		resources = _resources;
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
