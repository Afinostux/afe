#include "af_3dmath.hpp"
//
//	afVec4 functions
//
//utility
const afVec4 afVec4::identity = afVec4(0,0,0,1);
const afVec4 afVec4::zero = afVec4(0,0,0,0);
float	afVec4::len(){
	return sqrt(x*x + z*z + y*y + w*w);
}

hc4	afVec4::normalized(){
	float s = len();
	return afVec4(x/s, y/s, z/s, w/s);
}

void	afVec4::normalize(){
	float s = len();
	x /= s;
	y /= s;
	z /= s;
	w /= s;
}

hc4	afVec4::vectorPart(){
	return hc4(x, y, z, 0);
}

float	afVec4::scalarPart(){
	return w;
}


//addition
hc4	afVec4::operator+(const hc4& other){
	return afVec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

hc4	afVec4::operator+(const float other){
	return afVec4(x + other, y + other, z + other, w + other);
}

hc4&	afVec4::operator+=(const hc4& other){
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

hc4&	afVec4::operator+=(const float other){
	x += other;
	y += other;
	z += other;
	w += other;
	return *this;
}

//subtraction
hc4	afVec4::operator-(const hc4& other){
	return afVec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

hc4	afVec4::operator-(const float other){
	return afVec4(x - other, y - other, z - other, w - other);
}

hc4&	afVec4::operator-=(const hc4& other){
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

hc4&	afVec4::operator-=(const float other){
	x -= other;
	y -= other;
	z -= other;
	w -= other;
	return *this;
}

//negation
hc4	afVec4::operator-(){
	return afVec4(-x, -y, -z, -w);
}

//multiplication / division
hc4	afVec4::operator*(const float other){
	return afVec4(x * other, y * other, z * other, w * other);
}

hc4	afVec4::operator/(const float other){
	return afVec4(x / other, y / other, z / other, w / other);
}

hc4&	afVec4::operator*=(const float other){
	x *= other;
	y *= other;
	z *= other;
	w *= other;
	return *this;
}

hc4&	afVec4::operator/=(const float other){
	x /= other;
	y /= other;
	z /= other;
	w /= other;
	return *this;
}

//dotproduct
float	afVec4::operator*(const hc4& other){
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

//cross	product
hc4	afVec4::operator^(const hc4& other){
	return afVec4(y * other.z - z * other.y, x * other.z - z * other.x, x * other.y - y * other.x, 0);
}

//set	length
hc4	afVec4::operator%(const float other){
	return this->normalized() * other;
}

//
//	afMat4 funcs
//

const afMat4 afMat4::identity = 
afMat4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
 );

afMat4	afMat4::getTranspose(){
	return afMat4(
			m[0], m[1], m[2], m[3], 
			m[4], m[5], m[6], m[7], 
			m[8], m[9], m[10], m[11], 
			m[12], m[13], m[14], m[15]
		     );
}

void	afMat4::transpose(){
}

float	afMat4::determinant(){
}

afMat4	afMat4::getInverse(){
}

void	afMat4::invert(){
}

void	afMat4::getSubmat(float s[9], int i, int j){
}

float	afMat4::getSubmatDet(int i, int j){
}

afMat4&	afMat4::operator=(const afMat4& other){
}

afMat4	afMat4::operator+(const afMat4& other){
}

afMat4	afMat4::operator-(const afMat4& other){
}

afMat4	afMat4::operator*(const afMat4& other){
}

afMat4	afMat4::operator*(const float other){
}

