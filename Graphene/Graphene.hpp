#pragma once
#define PI 3.1415926

#include <random>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "ExceptionHandler.hpp"
#include "Engine.hpp"

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

		/********* Operator Definitions *********
		*|	+ add
		*|	- subtract
		*|	* mul by number
		*|	/ divide by number
		*|	* dot
		*|	^ cross
		*****************************************/

		NormCoord operator+(NormCoord a) {
			return NormCoord(this->x + a.x, this->y + a.y);
		}
		NormCoord operator-(NormCoord a) {
			return NormCoord(this->x - a.x, this->y - a.y);
		}
		NormCoord operator*(double a) {
			return NormCoord(this->x * a, this->y * a);
		}
		NormCoord operator/(double a) {
			return NormCoord(this->x / a, this->y / a);
		}
		double operator*(NormCoord a) {
			return this->x * a.x + this->y * a.y;
		}
		double operator^(NormCoord a) {
			return this->x * a.y - this->y * a.x;
		}
	};

	struct Vertex {
		int number = 0;
		string name = "";
		bool nameVisible = false;
		string value = "";
		bool valueVisible = false;
		NormCoord coord = NormCoord(dis(gen), dis(gen));

		Vertex(int _num, string _name, bool _nameVisible, string _value, bool _valueVisible, NormCoord _coord) {
			number = _num;
			name = _name;
			nameVisible = _nameVisible;
			value = _value;
			valueVisible = _valueVisible;
			coord = _coord;
		}

		Vertex(int _num) {
			number = _num;
			name = to_string(number);
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


	int totalVertex, totalEdge;
	vector<Vertex> verticies;
	vector<Edge> allEdges;
	vector<vector<Vertex> > structure;


	/***************************************************************************************************************/


	// Handles all the graph rendering, scales automatically depending on the texture assigned.
	class Renderer {
	public:
		Engine* engine_;
		Graphene* graphene = nullptr;
		ExceptionHandler eh;
		Engine::Element* graphElement = nullptr;
		Resources* resources = nullptr;

		Renderer(Graphene* _graphene, Engine* ui) {
			graphene = _graphene;
			resources = _graphene->resources;
			engine_ = ui;
		}

		void newGraphElement() {
			graphElement = new Engine::Element(engine_ ,nullptr, "graphElement");
			graphElement->getBody()->getBackgroundColor();
			int edgeID = 0;
			for (vector<Edge>::iterator edge = graphene->allEdges.begin(); edge != graphene->allEdges.end(); edge++) {
				Engine::Element* edgeElement = new Engine::Element(engine_, graphElement, "edge" + to_string(edgeID));
				edgeElement->getBody()->setLine(
					{ (float)edge->startingVertex->coord.x, 0 }, { (float)edge->startingVertex->coord.y, 0 },
					{ (float)edge->endingVertex->coord.x, 0 }, { (float)edge->endingVertex->coord.y, 0 },
					5, resources->colorGray);
				edgeID++;
			}
			int vertexID = 0;
			for (vector<Vertex>::iterator vertex = graphene->verticies.begin(); vertex != graphene->verticies.end(); vertex++) {
				Engine::Element* vertexElement = new Engine::Element(engine_, graphElement, "vertex" + to_string(edgeID),
					{ (float)vertex->coord.x, 0 }, { (float)vertex->coord.y, 0 }, { 0, 60 }, { 0, 60 }, 0.5, 0.5);
				vertexElement->getBody()->setCircle({ 0.0, 20 }, resources->colorGold, -4, resources->colorGoldenrod);
				//LegacyEngine::Element* vertexTextElement = new LegacyEngine::Element(vertexElement, "vertexText" + to_string(edgeID));
				//vertexTextElement->body->setSimpleText(vertex->name, resources->fontDefault, 30, resources->colorText, 0.5, 0.63);
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

		void initializeGraph(int _totalVertex, int _totalEdge, vector<pair<pair<int, int>, bool> >& inputEdges) {
			graphene->totalVertex = _totalVertex;
			graphene->totalEdge = _totalEdge;
			
			graphene->verticies.clear();
			graphene->allEdges.clear(); 
			graphene->structure.resize(graphene->totalVertex);

			for (int i = 0; i < graphene->totalVertex; i++) {
				graphene->verticies.emplace_back(Graphene::Vertex(i));
			}

			for (vector<pair<pair<int, int>, bool> >::iterator it = inputEdges.begin(); it != inputEdges.end(); it++) {
				int start = it->first.first, end = it->first.second;
				bool directed = it->second;
				Edge edge = Graphene::Edge(&graphene->verticies[start], &graphene->verticies[end], directed);

				graphene->allEdges.emplace_back(edge);
				graphene->structure[start].emplace_back(graphene->verticies[end]);
			}
		}

		void printPos() {
			cout << "----------------------------------\n";
			for (int i = 0; i < graphene->totalVertex; i++) {
				//
			}
		}

		const double c1 = 2;
		const double c2 = 1;
		const double c3 = 1;
		const double c4 = 0.1;

		double distance(Vertex& u, Vertex& v) {
			double dx = u.coord.x - v.coord.x;
			double dy = u.coord.y - v.coord.y;
			return sqrt(dx * dx + dy * dy);
		}

		NormCoord appealForce(Vertex& u, Vertex& v) {
			double dis = distance(u, v);
			if (dis == 0.0) return NormCoord(0, 0);
			double coeff = c1 * log10(dis / c2);
			return (v.coord - u.coord) * coeff;
		}

		NormCoord resistForce(Vertex& u, Vertex& v) {
			double dis = distance(u, v);
			if (dis == 0.0) return NormCoord(0, 0);
			double coeff = -c3 / sqrt(dis);
			return (v.coord - u.coord) * coeff;
		}

		void initializePos() {
			for (int i = 0; i < (int)graphene->verticies.size(); i++) {
				graphene->verticies[i].coord = NormCoord(dis(gen), dis(gen));
			}
		}

		void updatePos() {
			for (vector<Vertex>::iterator u = graphene->verticies.begin(); u != graphene->verticies.end(); u++) {
				NormCoord resultForce(0, 0);
				for (vector<Vertex>::iterator v = graphene->verticies.begin(); v != graphene->verticies.end(); v++) {
					resultForce = resultForce + appealForce(*u, *v);
				}
				for (vector<Vertex>::iterator v = graphene->structure[u->number].begin(); v != graphene->structure[u->number].end(); v++) {
					resultForce = resultForce + appealForce(*u, *v);
				}
				u->coord = u->coord + resultForce * c4;
			}
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
	Engine* ui;


	Graphene(Engine* engine_, Resources* _resources) {
		resources = _resources;
		ExceptionHandler eh;
		ui = engine_;
		renderer = new Renderer(this, ui);
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
