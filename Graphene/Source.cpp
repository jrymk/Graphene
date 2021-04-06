#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "Graphene.hpp"
#include "ExceptionHandler.hpp"
#include "UIEngine.hpp"
#include "Resources.hpp"

using namespace std;
using namespace sf;

int main() {
	ExceptionHandler eh;

	RenderWindow window(sf::VideoMode(800, 600), L"Graphene £\", Style::Default, ContextSettings(0, 0, 4, 0, 4, 0, false));
	window.setActive(false);

	Resources resources;
	resources.loadFont();

	UI ui(&window, &resources);
	Graphene* graphene = new Graphene(&resources);


	/*int v, e;
	cin >> v >> e;
	for (int i = 0; i < v; i++) {
		graphene->verticies.push_back(Graphene::Vertex());
		graphene->verticies[i].name = to_string(i);
	}
	for (int i = 0; i < e; i++) {
		int a, b;
		cin >> a >> b;
		graphene->edges.push_back(Graphene::Edge(&graphene->verticies[a], &graphene->verticies[b], false));
	}*/
	graphene->verticies.push_back(Graphene::Vertex());
	graphene->verticies[0].name = to_string(0);
	graphene->verticies.push_back(Graphene::Vertex());
	graphene->verticies[1].name = to_string(1);
	graphene->verticies.push_back(Graphene::Vertex());
	graphene->verticies[2].name = to_string(2);
	graphene->edges.push_back(Graphene::Edge(&graphene->verticies[0], &graphene->verticies[1], false));
	graphene->edges.push_back(Graphene::Edge(&graphene->verticies[1], &graphene->verticies[2], false));
	graphene->edges.push_back(Graphene::Edge(&graphene->verticies[2], &graphene->verticies[0], false));



	UI::Element* textElement = new UI::Element(ui.rootContainer, "textElement");
	textElement->body->setSimpleText("Lorem ipsum dolor sit amet, consectetur adipiscing elit.", resources.fontDefault, 30, resources.colorLightBlue, 0.0, 1.0);


	UI::Element* graphContainer = new UI::Element(ui.rootContainer, "graphContainer", { 0.5, 0 }, { 0.5, 0 }, { 1.0, 0 }, { 1.0, -80 }, 0.5, 0.5);
	graphContainer->sizingMode = UI::Element::SizingMode::RELATIVE_TO_H;


	ui.rootContainer->body->setNone();
	ui.rootContainer->body->setBackgroundColor(resources.colorBackground);

	Clock clock;


	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0.0, 0.0, (float)event.size.width, (float)event.size.height)));
			}

		}
		
		graphene->renderer->newGraphElement();
		graphene->renderer->graphElement->linkContainer(graphContainer);
		
		
		ui.render();

		graphene->verticies[0].coord.x = (sin(clock.getElapsedTime().asMilliseconds() / (float)1000) / 2) + 0.5;


		if (ui.interaction->mouseHoveredElement != nullptr) 
			cout << "hovered on " << ui.interaction->mouseHoveredElement->debugName << "\n";
		


		graphene->renderer->graphElement->deleteElement();
		
		eh.ok("hi!", __FILE__, __LINE__);

		eh.flushExceptionsToIOStream();

	}

	return 0;
}