#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include "utils/exceptionHandler.hpp"
#include "graphene/include.h"
#include "gui/include.h"



int main() {

	ASSERT(Gui::init());

	Graphene::Graph graph;
	Graphene::Core core(graph);


	while (!Gui::shouldClose()) {

		Gui::initFrame();

		Gui::Toolbar::show(&core);
		Gui::Input::show(&core);
		Gui::MainView::show(&core, &graph);
		
		Gui::render();

	}

	Gui::cleanup();
}