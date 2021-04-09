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
	bool showDebugBoundaries = false;
	bool showRenderingDebug = false;
	unsigned int circlePointCount = 20; // please make it multiple of 4
	Color* colorTranslucentYellow = new Color(255, 255, 0, 60);

	ContextSettings contextSettings = ContextSettings(0, 0, 
		4, // anti-aliasing
		0, 4, 0, false);

	Font* fontDefault = new Font();
	Font* fontMono = new Font();

	Color* colorTransparent = new Color(0, 0, 0, 0);
	Color* colorBackground = new Color(250, 250, 250, 255);
	Color* colorText = new Color(50, 50, 50);

	Color* colorIndianRed = new Color(205, 92, 92);
	Color* colorLightCoral = new Color(240, 128, 128);
	Color* colorSalmon = new Color(250, 128, 114);
	Color* colorDarkSalmon = new Color(233, 150, 122);
	Color* colorLightSalmon = new Color(255, 160, 122);
	Color* colorCrimson = new Color(220, 20, 60);
	Color* colorRed = new Color(255, 0, 0);
	Color* colorFireBrick = new Color(178, 34, 34);
	Color* colorDarkRed = new Color(139, 0, 0);
	Color* colorPink = new Color(255, 192, 203);
	Color* colorLightPink = new Color(255, 182, 193);
	Color* colorHotPink = new Color(255, 105, 180);
	Color* colorDeepPink = new Color(255, 20, 147);
	Color* colorMediumVioletRed = new Color(199, 21, 133);
	Color* colorPaleVioletRed = new Color(219, 112, 147);
	Color* colorCoral = new Color(255, 127, 80);
	Color* colorTomato = new Color(255, 99, 71);
	Color* colorOrangeRed = new Color(255, 69, 0);
	Color* colorDarkOrange = new Color(255, 140, 0);
	Color* colorOrange = new Color(255, 165, 0);
	Color* colorGold = new Color(255, 215, 0);
	Color* colorYellow = new Color(255, 255, 0);
	Color* colorLightYellow = new Color(255, 255, 224);
	Color* colorLemonChiffon = new Color(255, 250, 205);
	Color* colorLightGoldenrodYellow = new Color(250, 250, 210);
	Color* colorPapayaWhip = new Color(255, 239, 213);
	Color* colorMoccasin = new Color(255, 228, 181);
	Color* colorPeachPuff = new Color(255, 218, 185);
	Color* colorPaleGoldenrod = new Color(238, 232, 170);
	Color* colorKhaki = new Color(240, 230, 140);
	Color* colorDarkKhaki = new Color(189, 183, 107);
	Color* colorLavender = new Color(230, 230, 250);
	Color* colorThistle = new Color(216, 191, 216);
	Color* colorPlum = new Color(221, 160, 221);
	Color* colorViolet = new Color(238, 130, 238);
	Color* colorOrchid = new Color(218, 112, 214);
	Color* colorFuchsia = new Color(255, 0, 255);
	Color* colorMagenta = new Color(255, 0, 255);
	Color* colorMediumOrchid = new Color(186, 85, 211);
	Color* colorMediumPurple = new Color(147, 112, 219);
	Color* colorRebeccaPurple = new Color(102, 51, 153);
	Color* colorBlueViolet = new Color(138, 43, 226);
	Color* colorDarkViolet = new Color(148, 0, 211);
	Color* colorDarkOrchid = new Color(153, 50, 204);
	Color* colorDarkMagenta = new Color(139, 0, 139);
	Color* colorPurple = new Color(128, 0, 128);
	Color* colorIndigo = new Color(75, 0, 130);
	Color* colorSlateBlue = new Color(106, 90, 205);
	Color* colorDarkSlateBlue = new Color(72, 61, 139);
	Color* colorMediumSlateBlue = new Color(123, 104, 238);
	Color* colorGreenYellow = new Color(173, 255, 47);
	Color* colorChartreuse = new Color(127, 255, 0);
	Color* colorLawnGreen = new Color(124, 252, 0);
	Color* colorLime = new Color(0, 255, 0);
	Color* colorLimeGreen = new Color(50, 205, 50);
	Color* colorPaleGreen = new Color(152, 251, 152);
	Color* colorLightGreen = new Color(144, 238, 144);
	Color* colorMediumSpringGreen = new Color(0, 250, 154);
	Color* colorSpringGreen = new Color(0, 255, 127);
	Color* colorMediumSeaGreen = new Color(60, 179, 113);
	Color* colorSeaGreen = new Color(46, 139, 87);
	Color* colorForestGreen = new Color(34, 139, 34);
	Color* colorGreen = new Color(0, 128, 0);
	Color* colorDarkGreen = new Color(0, 100, 0);
	Color* colorYellowGreen = new Color(154, 205, 50);
	Color* colorOliveDrab = new Color(107, 142, 35);
	Color* colorOlive = new Color(128, 128, 0);
	Color* colorDarkOliveGreen = new Color(85, 107, 47);
	Color* colorMediumAquamarine = new Color(102, 205, 170);
	Color* colorDarkSeaGreen = new Color(143, 188, 139);
	Color* colorLightSeaGreen = new Color(32, 178, 170);
	Color* colorDarkCyan = new Color(0, 139, 139);
	Color* colorTeal = new Color(0, 128, 128);
	Color* colorAqua = new Color(0, 255, 255);
	Color* colorCyan = new Color(0, 255, 255);
	Color* colorLightCyan = new Color(224, 255, 255);
	Color* colorPaleTurquoise = new Color(175, 238, 238);
	Color* colorAquamarine = new Color(127, 255, 212);
	Color* colorTurquoise = new Color(64, 224, 208);
	Color* colorMediumTurquoise = new Color(72, 209, 204);
	Color* colorDarkTurquoise = new Color(0, 206, 209);
	Color* colorCadetBlue = new Color(95, 158, 160);
	Color* colorSteelBlue = new Color(70, 130, 180);
	Color* colorLightSteelBlue = new Color(176, 196, 222);
	Color* colorPowderBlue = new Color(176, 224, 230);
	Color* colorLightBlue = new Color(173, 216, 230);
	Color* colorSkyBlue = new Color(135, 206, 235);
	Color* colorLightSkyBlue = new Color(135, 206, 250);
	Color* colorDeepSkyBlue = new Color(0, 191, 255);
	Color* colorDodgerBlue = new Color(30, 144, 255);
	Color* colorCornflowerBlue = new Color(100, 149, 237);
	Color* colorRoyalBlue = new Color(65, 105, 225);
	Color* colorBlue = new Color(0, 0, 255);
	Color* colorMediumBlue = new Color(0, 0, 205);
	Color* colorDarkBlue = new Color(0, 0, 139);
	Color* colorNavy = new Color(0, 0, 128);
	Color* colorMidnightBlue = new Color(25, 25, 112);
	Color* colorCornsilk = new Color(255, 248, 220);
	Color* colorBlanchedAlmond = new Color(255, 235, 205);
	Color* colorBisque = new Color(255, 228, 196);
	Color* colorNavajoWhite = new Color(255, 222, 173);
	Color* colorWheat = new Color(245, 222, 179);
	Color* colorBurlyWood = new Color(222, 184, 135);
	Color* colorTan = new Color(210, 180, 140);
	Color* colorRosyBrown = new Color(188, 143, 143);
	Color* colorSandyBrown = new Color(244, 164, 96);
	Color* colorGoldenrod = new Color(218, 165, 32);
	Color* colorDarkGoldenrod = new Color(184, 134, 11);
	Color* colorPeru = new Color(205, 133, 63);
	Color* colorChocolate = new Color(210, 105, 30);
	Color* colorSaddleBrown = new Color(139, 69, 19);
	Color* colorSienna = new Color(160, 82, 45);
	Color* colorBrown = new Color(165, 42, 42);
	Color* colorMaroon = new Color(128, 0, 0);
	Color* colorWhite = new Color(255, 255, 255);
	Color* colorSnow = new Color(255, 250, 250);
	Color* colorHoneyDew = new Color(240, 255, 240);
	Color* colorMintCream = new Color(245, 255, 250);
	Color* colorAzure = new Color(240, 255, 255);
	Color* colorAliceBlue = new Color(240, 248, 255);
	Color* colorGhostWhite = new Color(248, 248, 255);
	Color* colorWhiteSmoke = new Color(245, 245, 245);
	Color* colorSeaShell = new Color(255, 245, 238);
	Color* colorBeige = new Color(245, 245, 220);
	Color* colorOldLace = new Color(253, 245, 230);
	Color* colorFloralWhite = new Color(255, 250, 240);
	Color* colorIvory = new Color(255, 255, 240);
	Color* colorAntiqueWhite = new Color(250, 235, 215);
	Color* colorLinen = new Color(250, 240, 230);
	Color* colorLavenderBlush = new Color(255, 240, 245);
	Color* colorMistyRose = new Color(255, 228, 225);
	Color* colorGainsboro = new Color(220, 220, 220);
	Color* colorLightGray = new Color(211, 211, 211);
	Color* colorSilver = new Color(192, 192, 192);
	Color* colorDarkGray = new Color(169, 169, 169);
	Color* colorGray = new Color(128, 128, 128);
	Color* colorDimGray = new Color(105, 105, 105);
	Color* colorLightSlateGray = new Color(119, 136, 153);
	Color* colorSlateGray = new Color(112, 128, 144);
	Color* colorDarkSlateGray = new Color(47, 79, 79);
	Color* colorBlack = new Color(0, 0, 0);

	Color* colorUIBoundsDebug = new Color(255, 180, 0, 50);
	Color* colorUIBoundsDebugSub = new Color(180, 180, 0, 50);


	Resources() {

	}

	void loadFont() {
		ExceptionHandler eh;
		eh.info("loading fonts", __FILE__, __LINE__);
		if (!fontDefault->loadFromFile(".\\Manrope-Medium.ttf")) eh.err("resource Manrope-Medium.ttf failed to load", __FILE__, __LINE__);
		if (!fontMono->loadFromFile(".\\consola.ttf")) eh.err("resource consola.ttf failed to load", __FILE__, __LINE__);
	}


};
