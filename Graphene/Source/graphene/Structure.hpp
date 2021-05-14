#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

namespace Graphene {

	struct Vec2f {
		float x;
		float y;
		Vec2f(float _x, float _y) {
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

		Vec2f operator+(Vec2f a) {
			return Vec2f(this->x + a.x, this->y + a.y);
		}

		void operator+=(Vec2f a) {
			this->x += a.x;
			this->y += a.y;
		}

		Vec2f operator-(Vec2f a) {
			return Vec2f(this->x - a.x, this->y - a.y);
		}

		void operator-=(Vec2f a) {
			this->x -= a.x;
			this->y -= a.y;
		}

		Vec2f operator*(float a) {
			return Vec2f(this->x * a, this->y * a);
		}

		void operator*=(float a) {
			this->x *= a;
			this->y *= a;
		}

		Vec2f operator/(float a) {
			return Vec2f(this->x / a, this->y / a);
		}

		void operator/=(float a) {
			this->x /= a;
			this->y /= a;
		}

		float operator*(Vec2f a) {
			return this->x * a.x + this->y * a.y;
		}

		float operator^(Vec2f a) {
			return this->x * a.y - this->y * a.x;
		}

		float length() {
			return sqrt(this->x * this->x + this->y * this->y);
		}

		Vec2f normalize() {
			if (length() <= 1e-9) return Vec2f(0, 0);
			return Vec2f(*this) / length();
		}

		friend std::ostream& operator<<(std::ostream& os, const Vec2f& nd) {
			return std::cout << "(" << nd.x << ", " << nd.y << ")";
		}
	};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    float genRandom() {
        return dis(gen);
    }

	class Vertex {

    public:
    	// TODO: fix up permissions
		bool pauseMove = false;
		int connectedComponent = 0;

    private:
		int number = 0;
		std::string name = "";
		bool nameVisible = false;
		std::string value = "";
		bool valueVisible = false;
		Vec2f coord = Vec2f(genRandom(), genRandom());
		Vec2f resultForce = Vec2f(0, 0);
		Vec2f normalized = Vec2f(genRandom(), genRandom());

		Vertex(int _num, std::string _name, bool _nameVisible, std::string _value, bool _valueVisible, Vec2f _coord) {
			number = _num;
			name = _name;
			nameVisible = _nameVisible;
			value = _value;
			valueVisible = _valueVisible;
			coord = _coord;
		}

	public:
        Vertex(int _num):
                coord(genRandom(), genRandom()),
                resultForce(0, 0),
                normalized(genRandom(), genRandom()) {
            number = _num;
            name = std::to_string(number);
        }

		Vec2f getCoord() {
			return coord;
		}

		void move(Vec2f v) {
			resultForce += v;
		}

		void flushMove(float c) {
			coord += resultForce * c;
			//std::cerr << getNumber() << " " << resultForce * c << std::endl;

			resultForce = Vec2f(0, 0);
		}

		int getNumber() {
			return number;
		}

	};

	struct Edge {
		std::string name = "";
		bool nameVisible = false;
		std::string value = "";
		bool valueVisible = false;

		Edge() {
		
		}
	};

}