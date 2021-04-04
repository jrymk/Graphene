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

int main() {
	ExceptionHandler eh;

	RenderWindow window(sf::VideoMode(800, 600), L"Graphene £\");
	
	Resources resources;
	resources.loadFont();

	Renderer renderer(&window);
	Graphene* graphene = new Graphene(&window);

	renderer.windowElement->backgroundColor = Color(250, 250, 250);

	Renderer::UIElement layout(renderer.windowElement);
	layout.x.set(0.9, 10);
	layout.y.set(0.5, 0);
	layout.w.set(0.8, 0);
	layout.h.set(0.0, 100);
	layout.originX.set(1.0, 0);
	layout.originY.set(0, 0);
	layout.backgroundColor = Color(200, 200, 255);
	
	Renderer::UIElement squareDemo(renderer.windowElement);
	squareDemo.x.set(0.4, 10);
	squareDemo.y.set(0.1, 0);
	squareDemo.w.set(0.5, 0);
	squareDemo.sizingMode = Renderer::UIElement::SizingMode::RELATIVE_TO_W;
	squareDemo.h.set(1.0, 0);
	squareDemo.originX.set(0, 0);
	squareDemo.originY.set(0, 0);


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

	Renderer::SimpleText text("Lorem ipsum dolor sit amet, consectetur adipiscing elit.", resources.fontDefault, 20, resources.colorText);
	Renderer::UIElement textElement(text.getTexture(), &layout, { 0.0, 0 }, { 1.0, 0 });

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

		eh.flushExceptionsToIOStream();

	}

	return 0;
}