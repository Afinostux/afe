
#pragma once
#include <math.h>
// homogeneous coordinates/vec4
typedef struct hc4 {
	float x, y, z, w;
	hc4(float x, float y, float z, float w) :
		x(x), y(y), z(z),w(w) {}
	//utility
	float len();
	hc4 normalized();
	void normalize();
	hc4 vectorPart();
	float scalarPart();
	//addition
	hc4 operator+(const hc4& other);
	hc4 operator+(const float other);
	hc4& operator+=(const hc4& other);
	hc4& operator+=(const float other);
	//subtraction
	hc4 operator-(const hc4& other);
	hc4 operator-(const float other);
	hc4& operator-=(const hc4& other);
	hc4& operator-=(const float other);
	//negation
	hc4 operator-();
	//multiplication / division
	hc4 operator*(const float other);
	hc4 operator/(const float other);
	hc4& operator*=(const float other);
	hc4& operator/=(const float other);
	//dot product
	float operator*(const hc4& other);
	//cross product
	hc4 operator^(const hc4& other);
	//set length
	hc4 operator%(const float other);
} vec4;


