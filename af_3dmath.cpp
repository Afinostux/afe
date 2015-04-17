#include "af_3dmath.hpp"
//
//	vec4 functions
//
//utility
float	vec4::len(){
	return sqrt(x*x + z*z + y*y + w*w);
}

hc4	vec4::normalized(){
	float s = len();
	return vec4(x/s, y/s, z/s, w/s);
}

void	vec4::normalize(){
	float s = len();
	x /= s;
	y /= s;
	z /= s;
	w /= s;
}

hc4	vec4::vectorPart(){
	return hc4(x, y, z, 0);
}

float	vec4::scalarPart(){
	return w;
}


//addition
hc4	vec4::operator+(const hc4& other){
	return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

hc4	vec4::operator+(const float other){
	return vec4(x + other, y + other, z + other, w + other);
}

hc4&	vec4::operator+=(const hc4& other){
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

hc4&	vec4::operator+=(const float other){
	x += other;
	y += other;
	z += other;
	w += other;
	return *this;
}

//subtraction
hc4	vec4::operator-(const hc4& other){
	return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

hc4	vec4::operator-(const float other){
	return vec4(x - other, y - other, z - other, w - other);
}

hc4&	vec4::operator-=(const hc4& other){
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

hc4&	vec4::operator-=(const float other){
	x -= other;
	y -= other;
	z -= other;
	w -= other;
	return *this;
}

//negation
hc4	vec4::operator-(){
	return vec4(-x, -y, -z, -w);
}

//multiplication / division
hc4	vec4::operator*(const float other){
	return vec4(x * other, y * other, z * other, w * other);
}

hc4	vec4::operator/(const float other){
	return vec4(x / other, y / other, z / other, w / other);
}

hc4&	vec4::operator*=(const float other){
	x *= other;
	y *= other;
	z *= other;
	w *= other;
	return *this;
}

hc4&	vec4::operator/=(const float other){
	x /= other;
	y /= other;
	z /= other;
	w /= other;
	return *this;
}

//dotproduct
float	vec4::operator*(const hc4& other){
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

//cross	product
hc4	vec4::operator^(const hc4& other){
	return vec4(y * other.z - z * other.y, x * other.z - z * other.x, x * other.y - y * other.x, 0);
}

//set	length
hc4	vec4::operator%(const float other){
	return this->normalized() * other;
}
