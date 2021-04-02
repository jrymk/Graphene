#include <random>
#include <iostream>
#include <iomanip>
#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <windows.h>

using namespace std;
using namespace sf;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<> dis(0.0, 1.0);

// Handles exceptions thrown by functions, can draw on window instead of printing on terminal for better visuals.
class ExceptionHandler {
public:
	enum class Level { INFO, OK, WARN, ERR, FATAL };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	struct Exception {
		Level level = Level::FATAL;
		string function = "NULL";
		string message = "NULL";
		int line = -1;
	};

	queue<Exception*> exceptionBuffer;

	void throwException(string _message, Level _level, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = _level;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void throwException(string _function, string _message, Level _level, int _line) {
		Exception* newException = new Exception;
		newException->function = _function;
		newException->message = _message;
		newException->level = _level;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void info(string _message, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::INFO;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void info(string _function, string _message, int _line) {
		Exception* newException = new Exception;
		newException->function = _function;
		newException->function = _function;
		newException->message = _message;
		newException->level = Level::INFO;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void ok(string _message, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::OK;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void ok(string _function, string _message, int _line) {
		Exception* newException = new Exception;
		newException->function = _function;
		newException->message = _message;
		newException->level = Level::OK;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void warn(string _message, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::WARN;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void warn(string _function, string _message, int _line) {
		Exception* newException = new Exception;
		newException->function = _function;
		newException->message = _message;
		newException->level = Level::WARN;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void err(string _message, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::ERR;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void err(string _function, string _message, int _line) {
		Exception* newException = new Exception;
		newException->function = _function;
		newException->message = _message;
		newException->level = Level::ERR;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void fatal(string _message, int _line) {
		Exception* newException = new Exception;
		newException->message = _message;
		newException->level = Level::FATAL;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	void fatal(string _function, string _message, int _line) {
		Exception* newException = new Exception;
		newException->function = _function;
		newException->message = _message;
		newException->level = Level::FATAL;
		newException->line = _line;
		exceptionBuffer.push(newException);
	}

	// Expected use case: Draw on screen without deleting buffer, print on terminal with deleting buffer in the end.
	bool getException(Exception* exception, bool deleteBuffer) {
		if (!exceptionBuffer.empty()) {
			exception->message = exceptionBuffer.front()->message;
			exception->level = exceptionBuffer.front()->level;
			exception->line = exceptionBuffer.front()->line;
			exception->function = exceptionBuffer.front()->function;
			delete exceptionBuffer.front();
			exceptionBuffer.pop();
			return true;
		}
		else return false;
	}

	void flushExceptions() {
		// Display exceptions to the terminal
		ExceptionHandler::Exception* exception = new ExceptionHandler::Exception;
		while (getException(exception, true)) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "[";
			switch (exception->level) {
			case ExceptionHandler::Level::INFO:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				std::cout << " INFO";
				break;
			case ExceptionHandler::Level::OK:
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << "  OK ";
				break;
			case ExceptionHandler::Level::WARN:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				std::cout << " WARN";
				break;
			case ExceptionHandler::Level::ERR:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
				std::cout << "ERROR";
				break;
			case ExceptionHandler::Level::FATAL:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				std::cout << "FATAL";
				break;
			}
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << "]";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			std::cout << " " << std::setw(3) << exception->line << " | ";
			if (exception->function != "NULL")
				std::cout << exception->function << " | ";
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			std::cout << exception->message << "\n";
		}
	}
};

ExceptionHandler eh;

class Graphene {
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
		Graphene* graphene;
		RenderTexture* renderTexture;
		sf::Font font;

		Renderer(Graphene* _graphene) {
			graphene = _graphene;
			renderTexture = nullptr;
			eh.info("renderer rendertexture is not defined, use Graphene::Renderer::assignRenderTexture(sf::RenderTexture) to assign a texture for rendering", __LINE__);
			eh.info("the current rendering system is temporary", __LINE__);

			if (!font.loadFromFile(".\\Manrope-Medium.ttf"))
				eh.err("font Manrope-Medium.ttf failed to load", __LINE__);
		}

		void assignRenderTexture(RenderTexture* _renderTexture) {
			renderTexture = _renderTexture;
		}

		Vector2f coordMapping(NormCoord coord) {
			if (coord.x < 0 || coord.x > 1)
				eh.warn("normalized coord x value is not in range 0.0 to 1.0", __LINE__);
			if (coord.y < 0 || coord.y > 1)
				eh.warn("normalized coord y value is not in range 0.0 to 1.0", __LINE__);

			int width = renderTexture->getSize().x;
			int height = renderTexture->getSize().y;
			int margin = 80;
			int mapSize = min(width - margin * 2, height - margin * 2);
			//eh.info("render texture size: " + to_string(width) + ", " + to_string(height), __LINE__);
			//eh.flushExceptions();
			return Vector2f(width / 2 - mapSize / 2 + coord.x * mapSize, height / 2 - mapSize / 2 + coord.y * mapSize);
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

			if (acos(delta.x / diagonalDistance) >= 0)
				rectShape->setRotation(180 - asin(delta.y / diagonalDistance) / 3.14159 * 180);
			else
				rectShape->setRotation(asin(delta.y / diagonalDistance) / 3.14159 * 180);
			rectShape->setPosition(pointA.x + delta.x / 2, pointA.y + delta.y / 2);
		}

		void render() {
			float lineWidth = 5;
			
			eh.flushExceptions();

			for (vector<Edge>::iterator edge = graphene->edges.begin(); edge != graphene->edges.end(); edge++) {
				RectangleShape rectShape;
				rectShape.setFillColor(Color(120, 120, 120));
				//eh.flushExceptions();
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
				text.setPosition(Vector2f(position.x, position.y + 10));
				renderTexture->draw(text);

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

		Graph(Graphene* _graphene) {
			graphene = _graphene;
		}
	};


	Renderer* renderer;
	Core* core;
	Graph* graph;


	Graphene() {
		renderer = new Renderer(this);
		core = new Core(this);
		graph = new Graph(this);
	}

};


int main() {
	RenderWindow window(sf::VideoMode(800, 600), "Graphene");

	Graphene* graphene = new Graphene();

	for (int i = 0; i < 3; i++) {
		graphene->verticies.push_back(Graphene::Vertex());
		graphene->verticies[i].name = to_string(i);
	}

	for (int i = 0; i < 3; i++) {
		graphene->edges.push_back(Graphene::Edge(&graphene->verticies[i], &graphene->verticies[(i + 1) % 3], false));
	}

	graphene->verticies[0].coord = Graphene::NormCoord(0.0, 1.0);
	graphene->verticies[1].coord = Graphene::NormCoord(1.0, 1.0);
	graphene->verticies[2].coord = Graphene::NormCoord(1.0, 0.0);


	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0, 0, event.size.width, event.size.height)));
			}

		}

		RenderTexture* graphTexture = new RenderTexture();
		if (!graphTexture->create(window.getSize().x, window.getSize().y/*, ContextSettings(0, 0, 2, 1, 1, 0, false)*/))
			eh.err("graph texture create failed", __LINE__);
		graphTexture->clear(Color(240, 240, 240, 255));
		graphTexture->setSmooth(true);
		
		eh.info("(" + to_string(Mouse::getPosition(window).x) + ", " + to_string(Mouse::getPosition(window).y) + ")", __LINE__);

		
		window.clear(Color(0, 0, 0, 255));

		//window.draw(Renderer

		// some basic testing
		graphene->renderer->assignRenderTexture(graphTexture);


		graphene->renderer->render();

		graphTexture->display();

		sf::Sprite sprite1;
		sprite1.setTexture(graphTexture->getTexture());



		window.draw(sprite1);

		window.display();

		delete graphTexture;

		eh.flushExceptions();

	}

	return 0;
}