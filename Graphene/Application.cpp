#include <iostream>
#include "SFML/Graphics.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "ExceptionHandler.hpp"
#include "Graphene.hpp"
#include "Resources.hpp"
#include "Engine.hpp"

// Use dedicated graphic cards by default (nVIDIA graphics)
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
// Use dedicated graphics card by default (AMD Radeon graphics)
extern "C" {
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


using namespace std;
using namespace sf;


// Render window on another thread
/*void render(UI* engine) {
	engine->window->setActive(true);
	while (engine->window->isOpen()) {
		engine->render();
	}
}*/


int main() {
	ExceptionHandler eh;

	Resources resources;
	resources.loadFont();

	RenderWindow window(sf::VideoMode(800, 600), L"Graphene Alpha", Style::Default, resources.contextSettings);
	//window.setActive(false);

	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	

	Engine* engine = new Engine(&window, &resources);
	Graphene* graphene = new Graphene(engine, &resources);

	//window.setActive(false);
	//Thread renderThread(&render, &engine);
	//renderThread.launch();
	


	/* Generate Graph  **********************************************************************/
	int totalVertex = 4;
	int totalEdge = (totalVertex) * (totalVertex - 1) / 2;

	vector<pair<pair<int, int>, bool> > inputEdges;
	for (int i = 0; i < totalVertex-1; i++) {
		for (int j = i + 1; j < totalVertex; j++) {
			inputEdges.emplace_back(make_pair(make_pair(i, j), false));
		}
	}

	graphene->core->initializeGraph(totalVertex, totalEdge, inputEdges);
	/****************************************************************************************/


	Engine::Element* graphContainerMargin = new Engine::Element(engine, engine->getRootElement(), "graphContainerMargin", 
		{ 0.5, 0 }, { 0.5, 0 }, { 0.0, 1 }, { 0.0, 1 }, 0.5, 0.5);
	graphContainerMargin->setSizingMode(Engine::Element::SizingMode::SHRINK_TO_FIT);
	graphContainerMargin->getBody()->setBackgroundColor(resources.colorLightGray);


	Engine::Element* graphContainer = new Engine::Element(engine, graphContainerMargin, "graphContainer", 
		{ 0.0, 50 }, { 0.0, 50 }, { 1.0, -100 }, { 1.0, -100 }, 0.0, 0.0);

	engine->getRootElement()->getBody()->setNone();
	engine->getRootElement()->getBody()->setBackgroundColor(resources.colorBackground);

	


	Clock clock;
	Time previousFrame = clock.getElapsedTime();


	Clock mainProfilerClock;
	//cout << "Main profiler - : " << mainProfilerClock.getElapsedTime().asMicroseconds() << "\n";
	mainProfilerClock.restart();

	graphene->core->initializePos();

	while (window.isOpen()) {

		//cout << "Main profiler - new loop: " << mainProfilerClock.getElapsedTime().asMicroseconds() << "\n";
		mainProfilerClock.restart();

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0.0, 0.0, (float)event.size.width, (float)event.size.height)));
			}

		}


		//cout << "Main profiler - poll events: " << mainProfilerClock.getElapsedTime().asMicroseconds() << "\n";
		mainProfilerClock.restart();

		//graphene->verticies[0].coord.x = (sin(clock.getElapsedTime().asMilliseconds() / (float)920) / 2) + 0.5;
		//graphene->verticies[0].coord.y = (cos(clock.getElapsedTime().asMilliseconds() / (float)920) / 2) + 0.5;
		//graphene->verticies[1].coord.x = (0.6 * sin(-clock.getElapsedTime().asMilliseconds() / (float)1760) / 2) + 0.5;
		//graphene->verticies[1].coord.y = (0.6 * cos(-clock.getElapsedTime().asMilliseconds() / (float)1760) / 2) + 0.5;
		//graphene->verticies[2].coord.x = (0.3 * sin(clock.getElapsedTime().asMilliseconds() / (float)370) / 2) + 0.5;
		//graphene->verticies[2].coord.y = (0.3 * cos(clock.getElapsedTime().asMilliseconds() / (float)370) / 2) + 0.5;
		

		graphene->core->updatePos();

		//cout << "Main profiler - graphene movement test: " << mainProfilerClock.getElapsedTime().asMicroseconds() << "\n";
		mainProfilerClock.restart();

		graphene->renderer->newGraphElement();
		graphene->renderer->graphElement->linkParentElement(graphContainer);

		//cout << "Main profiler - graphene spawn element: " << mainProfilerClock.getElapsedTime().asMicroseconds() << "\n";
		mainProfilerClock.restart();


		//UI.renderUI();
		engine->render();

		cout << (int)round((double)1000000 / (clock.getElapsedTime() - previousFrame).asMicroseconds()) << "fps\n";
		previousFrame = clock.getElapsedTime();

		/*if (engine.interaction->mouseHoveredElement != nullptr)
			//cout << "hovered on " << engine.interaction->mouseHoveredElement->debugName << "\n";*/

		graphene->renderer->graphElement->deleteElement();


		//cout << "Main profiler - end: " << mainProfilerClock.getElapsedTime().asMicroseconds() << "\n";
		mainProfilerClock.restart();
		//eh.ok("hi!", __FILE__, __LINE__);

		//eh.flushExceptionsToIOStream();
	}

	return 0;
}