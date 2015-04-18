
#pragma once
#include <stdio.h>
#include <math.h>
#include "af_math.hpp"
// homogeneous coordinates/vec4
typedef struct hc4 {
	static const hc4 identity;
	static const hc4 zero;
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
} afVec4;

// also quaternion?
typedef afVec4 afQuat;

//
// 	|	Xx	Xy	Xz	0	|
//	|					|
//	|	Yx	Yy	Yz	0	|
//	|					|
//	|	Zx	Zy	Zz	0	|
//	|					|
//	|	X	Y	Z	1	|
//
typedef struct mat4 {
	float m[16];
	static const mat4 identity;
	mat4() : m{
		1,	0,	0,	0,
		0,	1,	0,	0,
		0,	0,	1,	0,
		0,	0,	0,	1
		}
	{}

	mat4(const float ms[16])
	{
		for (int i = 16; i--;)
		{
			m[i] = ms[i];
		}
	}

	mat4 (
			float xx, float xy, float xz, float xw,
			float yx, float yy, float yz, float yw,
			float zx, float zy, float zz, float zw,
			float wx, float wy, float wz, float ww
	     ) : m{
			xx, xy, xz, xw,
			yx, yy, yz, yw,
			zx, zy, zz, zw,
			wx, wy, wz, ww
	}
	{}

	mat4 getTranspose();
	void transpose();
	float determinant();
	mat4 getInverse();
	void invert();
	void getSubmat(float s[9], int i, int j);
	float getSubmatDet(int i, int j);

	void rotate(float ex, float ey, float ez);
	void translate(float tx, float ty, float tz);

	mat4& operator=(const mat4& other);
	mat4 operator+(const mat4& other);
	mat4 operator-(const mat4& other);
	mat4 operator*(const mat4& other);
	mat4& operator*=(const mat4& other);
	mat4 operator*(const float other);
	float& operator[](const int other);
	void print(void) {
		printf("|~~~~~~(matrix)~~~~~~~~~~~~~~~~~~|\n|");
		for (int i = 0; i < 16; i++)
			printf("%-8.3f%s",m[i],((i+1)%4)?"":"|\n|");
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
	}
} afMat4;

