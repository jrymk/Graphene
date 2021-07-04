#pragma once

#include <time.h>
#include <random>
#include <sstream>

namespace graphene::tools::random {
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> normDist(0.0, 1.0);
std::uniform_int_distribution<> intDist16(0, 15);
bool launch = true;

float genRandomNorm() {
	if (launch) {
		launch = false;
		gen.seed(time(nullptr));
	}
	return normDist(gen);
}

int genRandom16() {
	if (launch) {
		launch = false;
		gen.seed(time(nullptr));
	}
	return intDist16(gen);
}

std::string generate64() {
	std::stringstream ss;
	int i;
	ss << std::hex;
	for (i = 0; i < 8; i++) {
		ss << genRandom16();
	}
	return ss.str();
}

} // namespace graphene::tools::random
