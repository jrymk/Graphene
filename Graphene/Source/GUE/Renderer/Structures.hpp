#pragma once

struct Vec2f {
	float x = 0.0f;
	float y = 0.0f;

	Vec2f() = default;

	Vec2f(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vec2f operator+(Vec2f addend) {
		return { this->x + addend.x, this->y + addend.y };
	}

	Vec2f operator-(Vec2f subtrahend) {
		return { this->x - subtrahend.x, this->y - subtrahend.y };
	}

};

struct Vec2u {
	unsigned int x = 0;
	unsigned int y = 0;

	Vec2u() = default;

	Vec2u(unsigned int x, unsigned int y) {
		this->x = x;
		this->y = y;
	}

	Vec2u operator+(Vec2u addend) {
		return { this->x + addend.x, this->y + addend.y };
	}

	Vec2u operator-(Vec2u subtrahend) {
		return { this->x - subtrahend.x, this->y - subtrahend.y };
	}
	
	Vec2f toFloat() {
		return { (float)this->x, (float)this->y };
	}
	
};

struct Vec2i {
	int x = 0;
	int y = 0;

	Vec2i() = default;

	Vec2i(int x, int y) {
		this->x = x;
		this->y = y;
	}

	Vec2i operator+(Vec2i addend) {
		return { this->x + addend.x, this->y + addend.y };
	}

	Vec2i operator-(Vec2i subtrahend) {
		return { this->x - subtrahend.x, this->y - subtrahend.y };
	}

	Vec2f toFloat() {
		return { (float)this->x, (float)this->y };
	}

	Vec2u toUnsigned() {
		return { (unsigned int)this->x, (unsigned int)this->y };
	}
	
};


struct Colorf {
public:
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;

	Colorf() = default;

	Colorf(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	
	Colorf setAlpha(float alpha) {
		return { this->r, this->g, this->b, alpha };
	}

};

struct Color {
public:
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0;

	Color() = default;

	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color setAlpha(unsigned char alpha) {
		return { this->r, this->g, this->b, alpha };
	} 

	Colorf toColorf() {
		return { this->r / 255.0f, this->g / 255.0f, this->b / 255.0f, this->a / 255.0f };
	}
};

struct AVec {
public :
	float rel = 0;
	float abs = 0;

	AVec() = default;

	AVec(float rel, float abs) {
		this->rel = rel;
		this->abs = abs;
	}

	AVec operator+(AVec addend) {
		return { this->rel + addend.rel, this->abs + addend.abs };
	}

	AVec operator-(AVec subtrahend) {
		return { this->rel - subtrahend.rel, this->abs - subtrahend.abs };
	}

	float evaluate(float parent) {
		return parent * rel + abs;
	}
	
};
