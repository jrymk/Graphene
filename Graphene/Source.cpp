#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "Graphene.hpp"
#include "ExceptionHandler.hpp"

using namespace std;
using namespace sf;

ExceptionHandler* eh = new ExceptionHandler();

int main() {
	RenderWindow window(sf::VideoMode(800, 600), "Graphene");

	Graphene* graphene = new Graphene(&window, eh);

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
			eh->err("graph texture create failed", __LINE__);
		graphTexture->clear(Color(240, 240, 240, 255));
		graphTexture->setSmooth(true);
		
		//eh->info("(" + to_string(Mouse::getPosition(window).x) + ", " + to_string(Mouse::getPosition(window).y) + ")", __LINE__);

		
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

		eh->flushExceptionsToIOStream();

	}

	return 0;
}