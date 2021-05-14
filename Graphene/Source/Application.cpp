#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <chrono>

#include "utils/ExceptionHandler.hpp"
#include "graphene/Include.h"
#include "gui/Include.h"

void updateGraphLoop(Graphene::Core &core, bool &update, bool &end) {
	while (!end) {
		if (update) {
			auto prevTime = std::chrono::high_resolution_clock::now();
			core.mutex.lock();
			core.updatePos();
			core.mutex.unlock();
			auto currTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> floatSecs = currTime - prevTime;
			std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::milliseconds>(floatSecs);
			//std::cerr << "Graph update time: " << micros.count() << "us\n";
		}
	}
}

int main() {

	Gui::init();

	Graphene::Graph graph;
	Graphene::Core core(graph);

	bool endBackgroundUpdateLoop = false;
	bool updateGraphAsync = true;
	std::thread backgroundUpdateThread(updateGraphLoop, std::ref(core), std::ref(updateGraphAsync), std::ref(endBackgroundUpdateLoop));
	backgroundUpdateThread.detach();

	while (!Gui::shouldClose()) {
		auto prevTime = std::chrono::high_resolution_clock::now();

		Gui::initFrame();

		core.mutex.lock();
		Gui::Toolbar::show(&core);
		Gui::Input::show(&core);
		Gui::GraphView::show(&core, &graph);

		Gui::render();
		core.mutex.unlock();

		updateGraphAsync = Gui::Toolbar::updateGraph;

		auto currTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> floatSecs = currTime - prevTime;
		std::chrono::microseconds micros = std::chrono::duration_cast<std::chrono::milliseconds>(floatSecs);
		//std::cout << "Render frame time: " << micros.count() << "us\n";
	}

	endBackgroundUpdateLoop = true;

	Gui::cleanup();
}