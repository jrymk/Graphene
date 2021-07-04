#pragma once

#include <string>
#include "Vector.hpp"
#include "../tools/Random.hpp"

namespace graphene::core {
class ConnectedComponent;

class Vertex {

  public:
	// TODO: fix up permissions
	bool pauseMove = false;
	int connectedComponent = 0;
	ConnectedComponent* component;
	std::string uid;
	char name[1024] = "";

  private:
	int number = 0;
	bool nameVisible = false;
	std::string value = "";
	bool valueVisible = false;
	Vec2f coord = Vec2f(graphene::tools::random::genRandomNorm(), graphene::tools::random::genRandomNorm());
	Vec2f resultForce = Vec2f(0, 0);
	Vec2f normalized = Vec2f(graphene::tools::random::genRandomNorm(), graphene::tools::random::genRandomNorm());

  public:
	Vertex(int _num) {
		number = _num;
		uid = graphene::tools::random::generate64();
	}

	void setCoord(Vec2f newCoord) { coord = newCoord; }

	Vec2f getCoord() { return coord; }

	void move(Vec2f v) { resultForce += v; }

	void flushMove(float c) {
		coord += resultForce * c;
		////std::cerr << getNumber() << " " << resultForce * c << std::endl;

		resultForce = Vec2f(0, 0);
	}

	void directMove(Vec2f v) { coord += v; }

	int getNumber() { return number; }

	bool componentifier_visited = false;
};
} // namespace graphene::core