#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#include <random>

namespace Graphene {

	namespace Structure {

		struct NormCoord {
			double x;
			double y;
			NormCoord(double _x, double _y) {
				x = _x;
				y = _y;
			}

			/********* Operator Definitions *********
			*|	+ add
			*|	- subtract
			*|	* mul by number
			*|	/ divide by number
			*|	* dot
			*|	^ cross
			*****************************************/

			NormCoord operator+(NormCoord a) {
				return NormCoord(this->x + a.x, this->y + a.y);
			}

			NormCoord operator-(NormCoord a) {
				return NormCoord(this->x - a.x, this->y - a.y);
			}

			NormCoord operator*(double a) {
				return NormCoord(this->x * a, this->y * a);
			}

			NormCoord operator/(double a) {
				return NormCoord(this->x / a, this->y / a);
			}

			double operator*(NormCoord a) {
				return this->x * a.x + this->y * a.y;
			}

			double operator^(NormCoord a) {
				return this->x * a.y - this->y * a.x;
			}

			double length() {
				return sqrt(this->x * this->x + this->y + this->y);
			}

			void normalize() {
				double len = this->length();
				this->x = this->x / len;
				this->y = this->y / len;
			}

			friend std::ostream& operator<<(std::ostream& os, const NormCoord& nd) {
				return std::cout << "(" << nd.x << ", " << nd.y << ")";
			}
		};


		struct Vertex {

			static float genRandom() {
				static std::random_device rd;
				static std::mt19937 gen(rd());
				static std::uniform_real_distribution<> dis(0.0, 1.0);
				return dis(gen);
			}

			int number = 0;
			std::string name = "";
			bool nameVisible = false;
			std::string value = "";
			bool valueVisible = false;
			NormCoord coord = NormCoord(genRandom(), genRandom());

			Vertex(int _num, std::string _name, bool _nameVisible, std::string _value, bool _valueVisible, NormCoord _coord) {
				number = _num;
				name = _name;
				nameVisible = _nameVisible;
				value = _value;
				valueVisible = _valueVisible;
				coord = _coord;
			}

			Vertex(int _num) {
				number = _num;
				name = std::to_string(number);
			}
		};

		struct Edge {
			std::string name = "";
			bool nameVisible = false;
			std::string value = "";
			bool valueVisible = false;
			Vertex* startingVertex = nullptr;
			Vertex* endingVertex = nullptr;
			bool directed = false;

			Edge(Vertex* _startingVertex, Vertex* _endingVertex, bool _directed) {
				startingVertex = _startingVertex;
				endingVertex = _endingVertex;
				directed = _directed;
			}
		};

		// Handles the structure of a graph, and support for multiple graphs, graph stepping, highlighting, etc.

	};
}