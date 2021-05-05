#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include "utils/ExceptionHandler.hpp"
#include "graphene/Include.h"
#include "gui/Include.h"



int main() {

	Gui::init();
	
	Graphene::Graph graph;
	Graphene::Core core(graph);


	while (!Gui::shouldClose()) {

		Gui::initFrame();

		Gui::Toolbar::show(&core);
		Gui::Input::show(&core);
		Gui::GraphView::show(&core, &graph);
		
		Gui::render();

	}

	Gui::cleanup();
}