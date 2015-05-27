
#pragma once
#include <stdio.h>
#include <math.h>
#include "afmath.hpp"
// homogeneous coordinates/vec4
typedef struct hc4 {
	static const hc4 identity;
	static const hc4 zero;
	float x, y, z, w;

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
	hc4 operator-(const hc4& other) const{
		hc4 result = {x - other.x, y - other.y, z - other.z, w - other.w};
		return result;
	}

	hc4 operator-(const float other) {
		hc4 result = {x - other, y - other, z - other, w};
		return result;
	}
	hc4& operator-=(const hc4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}
	hc4& operator-=(const float other) {
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}
	//negation
	hc4 operator-();

	//multiplication / division
	inline
	hc4 operator*(const float other) const
	{
		hc4 result = {x * other, y * other, z * other, w};
		return result;
	}

	inline
	hc4 operator/(const float other) const
	{
		float o = 1.f/other;
		hc4 result = {x * o, y * o, z * o, w};
		return result;
	}

	inline
	hc4& operator*=(const float other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	inline
	hc4& operator/=(const float other)
	{
		float o = 1.f/other;
		x *= o;
		y *= o;
		z *= o;
		return *this;
	}

	//dot product
	float operator*(const hc4& other);
	//cross product
	hc4 operator^(const hc4& other);
	//set length
	hc4 operator%(const float other);
} afVec4;

inline
afVec4
V4( float x, float y, float z, float w )
{
	afVec4 result;
	result.x = x;
	result.y = y;
 	result.z = z;
 	result.w = w;
	return result;
}

inline
afVec4
V4( float V[4] )
{
	afVec4 result;
	result.x = V[0];
	result.y = V[1];
 	result.z = V[2];
 	result.w = V[3];
	return result;
}


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
#if 0
	mat4 getInverse();
	void invert();
	void getSubmat(float s[9], int i, int j);
	float getSubmatDet(int i, int j);
#endif

	void frustum(float left, float right,
		float bottom, float top,
		float near, float far);
	void perspective(float fovy, float aspect,
		float znear, float zfar);

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

struct quat;
inline
afVec4 operator^(const afVec4& a, const quat& b);

inline
afVec4 operator^(const quat& a, const afVec4& b);

//quat is like a vec4, except that it needs to always be normalized across all 4 axes.
typedef struct quat {
	float x, y, z, w;
	static const quat identity;
	float len();
	float sqlen();
	int isNormal();
	void normalize();
	quat normalized();
	hc4 vectorPart();
	float scalarPart();
	quat rotateByQuat(const quat& other)const;
	quat rotateByAngleAxis(hc4 other, float radians);
	quat rotateByEuler(float p, float y, float r);
	mat4 toMatrix();

	inline
	afVec4 crossByVector(const afVec4& other) const
	{
		afVec4 result;
		result.x = y * other.z - z * other.y;
	   	result.y = x * other.z - z * other.x;
	   	result.z = x * other.y - y * other.x;
		result.w = 0;
		return result;
	}

	inline
	afVec4 transform(const afVec4& other) const
	{
#if 1
		afVec4 t = crossByVector(other)*2.f;
		return (t*w) + crossByVector(t) + other;
#else
		return (*this)^other^(-(*this));
#endif
	}

	inline
	quat operator*(const float other)
	{
		quat result;
		result.x = other*x;
		result.y = other*y;
		result.z = other*z;
		result.w = other*w;
		return result;
	}

	inline
	quat& operator*=(const float other)
	{
		// TODO(afox): proportional rotation?
		return *this;
	}

	inline
	quat& operator+=(const quat& other)
	{
		*this = this->rotateByQuat(other);
		return *this;
	}

	inline
	quat operator-() const
	{
		quat result = {
			-x, -y, -z, -w
		};
		return result;
	}

} afQuat;

inline
afVec4 operator^(const afVec4& a, const afQuat& b)
{
	afVec4 result = {
		a.y * b.z - a.z * b.y,
		a.x * b.z - a.z * b.x,
 		a.x * b.y - a.y * b.x,
 		0
	};
	return result;
}

inline
afVec4 operator^(const afQuat& a, const afVec4& b)
{
	afVec4 result = {
		a.y * b.z - a.z * b.y,
		a.x * b.z - a.z * b.x,
 		a.x * b.y - a.y * b.x,
 		0
	};
	return result;
}

inline
afQuat
QUAT( float Q[4] )
{
	afQuat result;
	result.x = Q[0];
	result.y = Q[1];
	result.z = Q[2];
	result.w = Q[3];
	return result;
}

//
//	coordinate
//
typedef struct coord {
	afVec4 position;
	afQuat rotation;
	
	inline coord localize(const coord& o) const
	{
		//TODO(afox): stub
		return *this;
	}

	inline coord delocalize(const coord& o) const
	{
		//TODO(afox): stub
		return o;
	}

	inline afVec4 transform(const afVec4& o) const
	{
		afVec4 result = rotation.transform(o);
		result += position;
		return result;
	}

	inline coord operator*(const float o) const
	{
		coord result;
		result.rotation = rotation;
		result.position = position * o;
		return result;
	}

	inline coord& operator*=(const float o)
	{
		position = position*o;
		return *this;
	}

	inline coord operator+(const coord& o) const
	{
		coord result;
		result.rotation = rotation.rotateByQuat(o.rotation);
		result.position = o.rotation.transform(position) + o.position;
		return result;
	}

	inline coord& operator+=(const coord& o)
	{
		rotation = rotation.rotateByQuat(o.rotation);
		position = o.rotation.transform(position) + o.position;
		return *this;
	}

} afCoord;
