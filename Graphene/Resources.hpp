#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>

#include "ExceptionHandler.hpp"

using namespace std;
using namespace sf;

// this will be a crude resource manager, no management in fact
class Resources {
public:
	ExceptionHandler* eh;

	Font* fontDefault = new Font();
	Font* fontMono = new Font();

	Color* colorText = new Color(50, 50, 50);

	Resources(ExceptionHandler* _eh) {
		eh = _eh;
	}


	void loadFont() {
		if (!fontDefault->loadFromFile(".\\Manrope-Medium.ttf")) eh->err("resource Manrope-Medium.ttf failed to load", __FILE__, __LINE__);
		if (!fontMono->loadFromFile(".\\consola.ttf")) eh->err("resource consola.ttf failed to load", __FILE__, __LINE__);
	}
};
