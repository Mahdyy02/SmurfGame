#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

class Vector2D{

public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& add(const Vector2D vec);
	Vector2D& subtract(const Vector2D vec);
	Vector2D& multiply(const Vector2D vec);
	Vector2D& divide(const Vector2D vec);

	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	friend std::ostream& operator<<(std::ostream os, const Vector2D vec);
};

#endif // !VECTOR2D_H
