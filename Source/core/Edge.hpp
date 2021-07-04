#pragma once

#include <string>
#include "../tools/Random.hpp"

namespace graphene::core {
class Edge {
  public:
	std::string name = "";
	bool nameVisible = false;
	std::string value = "";
	bool valueVisible = false;

	Edge() {}
};
} // namespace graphene::core