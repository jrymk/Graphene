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

	Resources resources;
	resources.loadFont();

	UIEngine uiEngine(&window);
	Graphene* graphene = new Graphene();


	int v, e;
	cin >> v >> e;
	for (int i = 0; i < v; i++) {
		graphene->verticies.push_back(Graphene::Vertex());
		graphene->verticies[i].name = to_string(i);
	}
	for (int i = 0; i < e; i++) {
		int a, b;
		cin >> a >> b;
		graphene->edges.push_back(Graphene::Edge(&graphene->verticies[a], &graphene->verticies[b], false));
	}

	/*UIEngine::SimpleText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.", resources.fontDefault, 20, resources.colorText);
	UIEngine::UIElement textElement(text.getTexture(), &layout, { 0.0, 0 }, { 1.0, 0 });*/


	uiEngine.rootContainer->body->setNone();
	uiEngine.rootContainer->body->setBackgroundColor(resources.colorBackground);


	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0.0, 0.0, (float)event.size.width, (float)event.size.height)));
			}

		}

		graphene->renderer->updateGraphElement();
		graphene->renderer->graphElement->linkContainer(uiEngine.rootContainer);

		uiEngine.render();
	
		delete graphene->renderer->graphElement;
		//delete mouseHoveredItemDisplay;

		eh.flushExceptionsToIOStream();

	}

	return 0;
}