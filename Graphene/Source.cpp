#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "Graphene.hpp"
#include "ExceptionHandler.hpp"
#include "Renderer.hpp"
#include "Resources.hpp"

using namespace std;
using namespace sf;
\
int main() {
	ExceptionHandler* eh = new ExceptionHandler();

	RenderWindow window(sf::VideoMode(800, 600), L"Graphene £\");
	Renderer renderer(&window, eh);
	
	Resources* resources = new Resources(eh);
	resources->loadFont();

	Graphene* graphene = new Graphene(&window, eh);

	Renderer::UIElement* layout = new Renderer::UIElement(renderer.windowElement, eh);
	layout->x->set(0.9, 10);
	layout->y->set(0.5, 0);
	layout->w->set(0.8, 0);
	layout->h->set(0.0, 100);
	layout->originX->set(1.0, 0);
	layout->originY->set(0, 0);
	
	Renderer::UIElement* squareDemo = new Renderer::UIElement(renderer.windowElement, eh);
	squareDemo->x->set(0.4, 10);
	squareDemo->y->set(0.1, 0);
	squareDemo->w->set(0.5, 0);
	squareDemo->sizingMode = Renderer::UIElement::SizingMode::RELATIVE_TO_W;
	squareDemo->h->set(1.0, 0);
	squareDemo->originX->set(0, 0);
	squareDemo->originY->set(0, 0);

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

	Renderer::SimpleText text = Renderer::SimpleText(eh);
	text.setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit.", resources->fontDefault, 20, resources->colorText);
	Renderer::UIElement textElement(text.getTexture(), layout, eh);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0.0, 0.0, (float)event.size.width, (float)event.size.height)));
			}

		}


		renderer.render();

		eh->flushExceptionsToIOStream();

	}

	return 0;
}