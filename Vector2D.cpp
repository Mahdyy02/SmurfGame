#include "Vector2D.h"

Vector2D::Vector2D() {
	this->x = 0.0;
	this->y = 0.0;
}

Vector2D::Vector2D(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2D& Vector2D::add(const Vector2D vec) {
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2D& Vector2D::subtract(const Vector2D vec) {
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2D& Vector2D::multiply(const Vector2D vec) {
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2D& Vector2D::divide(const Vector2D vec) {
	if (vec.x == 0.0 || vec.y == 0.0) {
		std::cerr << "Zero Divison Error" << std::endl;
		exit(EXIT_FAILURE);
	}

	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}


Vector2D& operator+(Vector2D& v1, const Vector2D& v2) {
	return v1.add(v2);
}

Vector2D& operator-(Vector2D& v1, const Vector2D& v2) {
	return v1.subtract(v2);
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2) {
	return v1.multiply(v2);
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2) {
	return v1.divide(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec) {
	return this->add(vec);
}

Vector2D& Vector2D::operator-=(const Vector2D& vec) {
	return this->subtract(vec);
}

Vector2D& Vector2D::operator*=(const Vector2D& vec) {
	return this->multiply(vec);
}

Vector2D& Vector2D::operator/=(const Vector2D& vec) {
	return this->divide(vec);
}

Vector2D& Vector2D::operator*(const int& i) {
	this->x *= i;
	this->y *= i;

	return *this;
}

Vector2D& Vector2D::zero() {
	this->x = 0;
	this->y = 0;

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
	os << "(" << vec.x << "," << vec.y << ")";
	return os;
}

Vector2D& Vector2D::normalize() {
	float magnitude = sqrt(x * x + y * y);
	if (magnitude != 0.0f) {
		x /= magnitude;
		y /= magnitude;
	}
	return *this;
}