#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "Graphene.hpp"
#include "ExceptionHandler.hpp"
#include "UIEngine.hpp"
#include "Resources.hpp"
#include "UI.hpp"

using namespace std;
using namespace sf;


/*void render(LegacyUI* ui) {
	ui->window->setActive(true);
	while (ui->window->isOpen()) {
		ui->render();
	}
}*/


int main() {
	ExceptionHandler eh;

	RenderWindow window(sf::VideoMode(800, 600), L"Graphene £\", Style::Default, ContextSettings(0, 0, 4, 0, 4, 0, false));
	window.setActive(false);

	Resources resources;
	resources.loadFont();

	LegacyUI legacyUI(&window, &resources);
	Graphene* graphene = new Graphene(&resources);

	//window.setActive(false);
	//Thread renderThread(&render, &ui);
	//renderThread.launch();
	
	int v = 5;
	for (int i = 0; i < v; i++) {
		graphene->verticies.push_back(Graphene::Vertex());
		graphene->verticies[i].name = to_string(i);
	}
	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++) {
			if (i != j) {
				graphene->edges.push_back(Graphene::Edge(&graphene->verticies[i], &graphene->verticies[j], false));
			}
		}
	}
	/*for (int i = 0; i < e; i++) {
		int a, b;
		cin >> a >> b;
		graphene->edges.push_back(Graphene::Edge(&graphene->verticies[a], &graphene->verticies[b], false));
	}*/
	/*graphene->verticies.push_back(Graphene::Vertex());
	graphene->verticies[0].name = to_string(0);
	graphene->verticies.push_back(Graphene::Vertex());
	graphene->verticies[1].name = to_string(1);
	graphene->verticies.push_back(Graphene::Vertex());
	graphene->verticies[2].name = to_string(2);*/
	/*graphene->edges.push_back(Graphene::Edge(&graphene->verticies[0], &graphene->verticies[1], false));
	graphene->edges.push_back(Graphene::Edge(&graphene->verticies[1], &graphene->verticies[2], false));
	graphene->edges.push_back(Graphene::Edge(&graphene->verticies[2], &graphene->verticies[0], false));
	*/


	LegacyUI::Element* textElement = new LegacyUI::Element(legacyUI.rootContainer, "textElement");
	textElement->body->setSimpleText("Lorem ipsum dolor sit amet, consectetur adipiscing elit.", resources.fontDefault, 30, resources.colorLightBlue, 0.0, 1.0);


	LegacyUI::Element* graphContainerMargin = new LegacyUI::Element(legacyUI.rootContainer, "graphContainerMargin", { 0.5, 0 }, { 0.5, 0 }, { 0.0, 1 }, { 0.0, 1 }, 0.5, 0.5);
	graphContainerMargin->sizingMode = LegacyUI::Element::SizingMode::SHRINK_TO_FIT;
	graphContainerMargin->body->setBackgroundColor(resources.colorLightGray);


	LegacyUI::Element* graphContainer = new LegacyUI::Element(graphContainerMargin, "graphContainer", { 0.0, 50 }, { 0.0, 50 }, { 1.0, -100 }, { 1.0, -100 }, 0.0, 0.0);

	legacyUI.rootContainer->body->setNone();
	legacyUI.rootContainer->body->setBackgroundColor(resources.colorBackground);

	

	graphene->renderer->newGraphElement();
	graphene->renderer->graphElement->linkContainer(graphContainer);
	

	Clock clock;
	Time previousFrame = clock.getElapsedTime();

	//UI2 ui2(&window, &resources);


	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0.0, 0.0, (float)event.size.width, (float)event.size.height)));
			}

		}


		/*graphene->renderer->newGraphElement();
		graphene->renderer->graphElement->linkContainer(graphContainer);


		graphene->verticies[0].coord.x = (sin(clock.getElapsedTime().asMilliseconds() / (float)920) / 2) + 0.5;
		graphene->verticies[0].coord.y = (cos(clock.getElapsedTime().asMilliseconds() / (float)920) / 2) + 0.5;
		graphene->verticies[1].coord.x = (0.6 * sin(-clock.getElapsedTime().asMilliseconds() / (float)1760) / 2) + 0.5;
		graphene->verticies[1].coord.y = (0.6 * cos(-clock.getElapsedTime().asMilliseconds() / (float)1760) / 2) + 0.5;
		graphene->verticies[2].coord.x = (0.3 * sin(clock.getElapsedTime().asMilliseconds() / (float)370) / 2) + 0.5;
		graphene->verticies[2].coord.y = (0.3 * cos(clock.getElapsedTime().asMilliseconds() / (float)370) / 2) + 0.5;
		*/

		//ui2.renderUI();
		legacyUI.render();

		cout << (int)round((double)1000000 / (clock.getElapsedTime() - previousFrame).asMicroseconds()) << "fps\n";
		previousFrame = clock.getElapsedTime();

		/*if (ui.interaction->mouseHoveredElement != nullptr)
			cout << "hovered on " << ui.interaction->mouseHoveredElement->debugName << "\n";*/

		graphene->renderer->graphElement->deleteElement();


		//eh.ok("hi!", __FILE__, __LINE__);

		//eh.flushExceptionsToIOStream();

	}

	return 0;
}