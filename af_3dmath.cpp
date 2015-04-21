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
			m[0],m[4],m[8],m[12],
			m[1],m[5],m[9],m[13],
			m[2],m[6],m[10],m[14],
			m[3],m[7],m[11],m[15]
		     );
}

void	afMat4::transpose(){
	float swap;
	swap = m[4]; m[4] = m[1]; m[1] = swap;
	swap = m[8]; m[8] = m[2]; m[2] = swap;
	swap = m[9]; m[9] = m[6]; m[6] = swap;
	swap = m[14]; m[14] = m[11]; m[11] = swap;
	swap = m[13]; m[13] = m[7]; m[7] = swap;
	swap = m[12]; m[12] = m[3]; m[3] = swap;
}

#if 0
float	afMat4::determinant(){
	float det, result = 0, i = 1;
	int n;
	for (n = 0; n < 4; n++, i *= -1)
	{
		det = getSubmatDet(n, 0);
		result += m[n] * det * i;
	}
	return result;
}

afMat4	afMat4::getInverse(){
	afMat4 result(this->m);
	float det = determinant();
	int sign;

	if (fabs(det) < 0.0005) return result;

	for (int y = 4; y--;){
		for (int x = 4; x--;){
			sign = 1 - ((x+y)%2) *2;
			result.m[x+y*4] = (getSubmatDet(x,y)*sign)/det;
		}
	}
	return result;
}

void	afMat4::invert(){
	*this = getInverse();
}

void	afMat4::getSubmat(float s[9], int i, int j){
	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
			s[ x + y * 3 ] =
				m[ ( i + x ) % 4 + ( ( j + y ) % 4 ) * 4 ];
}

//	0	1	2
//
//	3	4	5
//
//	6	7	8

float	afMat4::getSubmatDet(int i, int j){
	float s[9];
	getSubmat(s, i, j);

	return	s[0] * ( s[4] * s[8] - s[7] * s[5] ) 
	-	s[3] * ( s[1] * s[8] - s[2] * s[7] ) 
	+	s[6] * ( s[1] * s[5] - s[2] * s[4] );
}
#endif

void afMat4::frustum(float left, float right,
		float bottom, float top, float near,
		float far){
	float 
	twicenear = 2 * near,
	width = right - left,
	height = top - bottom,
	depth = far - near;
	m[0] = twicenear / width;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = twicenear / height;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = (right + left) / width;
	m[9] = (top + bottom) / height;
	m[10] = (-far - near) / depth;
	m[11] = -1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = (-twicenear * far) / depth;
	m[15] = 0.0f;
}

void afMat4::perspective(float fovy, float aspect,
		float znear, float zfar){
	float
	ymax = znear * tanf(fovy * (M_PI / 360.0)),
	xmax = ymax * aspect;
	frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

//	0	1	2	3
//
//	4	5	6	7
//
//	8	9	10	11
//
//	12	13	14	15
void afMat4::rotate(float ex, float ey, float ez){
	float
/*a*/	cosx = cosf(ex),
/*b*/	sinx = sinf(ex),
/*c*/	cosy = cosf(ey),
/*d*/	siny = sinf(ey),
/*e*/	cosz = cosf(ez),
/*f*/	sinz = sinf(ez),
	cosxsiny = cosx * siny,
	sinxsiny = sinx * siny;

	afMat4 temp;
	temp[0] = cosy * cosz ;
	temp[4] = -cosy * sinz ;
	temp[8] = -siny ;

	temp[1] = -sinxsiny* cosz + cosx * sinz ;
	temp[5] = sinxsiny* sinz + cosx * cosz ;
	temp[9] = -sinx * cosy ;

	temp[2] = cosxsiny* cosz + sinx * sinz ;
	temp[6] = -cosxsiny* sinz + sinx * cosz ;
	temp[10] = cosx * cosy ;

	*this *= temp;

#if 0
	afMat4 temp;
	afMat4 macc;
	int iacc = 0;;
	if (fabs(ex) > 0.00001) {
		macc[5] = cosf(ex);
		macc[10] = cosf(ex);
		macc[6] = sinf(ex);
		macc[9] = -sinf(ex);
		iacc++;
	}

	if (fabs(ey) > 0.00001) {
		temp[0] = cosf(ey);
		temp[10] = cosf(ey);
		temp[2] = -sinf(ey);
		temp[8] = sinf(ey);
		macc *= temp;
		if (iacc) temp = afMat4::identity;
	}

	if (fabs(ez) > 0.00001) {
		temp[0] = cosf(ez);
		temp[5] = cosf(ez);
		temp[1] = sinf(ez);
		temp[4] = -sinf(ez);
		iacc = 99;
	}

	if (iacc == 99)
		*this *= macc * temp;
	else
		*this *= macc;
#endif
}

void afMat4::translate(float tx, float ty, float tz){
	afMat4 temp;
	temp[12] = tx;
	temp[13] = ty;
	temp[14] = tz;
	*this *= temp;
}


afMat4&	afMat4::operator=(const afMat4& other){
	for (int i = 16; i--;) m[i] = other.m[i];
	return *this;
}

afMat4	afMat4::operator+(const afMat4& other){
	return afMat4( 
		m[0] + other.m[0],
		m[1] + other.m[1],
		m[2] + other.m[2],
		m[3] + other.m[3],
		m[4] + other.m[4],
		m[5] + other.m[5],
		m[6] + other.m[6],
		m[7] + other.m[7],
		m[8] + other.m[8],
		m[9] + other.m[9],
		m[10] + other.m[10],
		m[11] + other.m[11],
		m[12] + other.m[12],
		m[13] + other.m[13],
		m[14] + other.m[14],
		m[15] + other.m[15]);
}

afMat4	afMat4::operator-(const afMat4& other){
	return afMat4( 
		m[0] - other.m[0],
		m[1] - other.m[1],
		m[2] - other.m[2],
		m[3] - other.m[3],
		m[4] - other.m[4],
		m[5] - other.m[5],
		m[6] - other.m[6],
		m[7] - other.m[7],
		m[8] - other.m[8],
		m[9] - other.m[9],
		m[10] - other.m[10],
		m[11] - other.m[11],
		m[12] - other.m[12],
		m[13] - other.m[13],
		m[14] - other.m[14],
		m[15] - other.m[15]);
}

//	0	1	2	3
//
//	4	5	6	7
//
//	8	9	10	11
//
//	12	13	14	15

afMat4	afMat4::operator*(const afMat4& other){
	return afMat4(
		m[0]*other.m[0]  + m[1]*other.m[4]  + m[2]*other.m[8]   + m[3]*other.m[12],	//xx
		m[0]*other.m[1]  + m[1]*other.m[5]  + m[2]*other.m[9]   + m[3]*other.m[13],	//xy
		m[0]*other.m[2]  + m[1]*other.m[6]  + m[2]*other.m[10]  + m[3]*other.m[14],	//xz
		m[0]*other.m[3]  + m[1]*other.m[7]  + m[2]*other.m[11]  + m[3]*other.m[15],	//xw
		m[4]*other.m[0]  + m[5]*other.m[4]  + m[6]*other.m[8]   + m[7]*other.m[12],	//yx
		m[4]*other.m[1]  + m[5]*other.m[5]  + m[6]*other.m[9]   + m[7]*other.m[13],	//yy
		m[4]*other.m[2]  + m[5]*other.m[6]  + m[6]*other.m[10]  + m[7]*other.m[14],	//yz
		m[4]*other.m[3]  + m[5]*other.m[7]  + m[6]*other.m[11]  + m[7]*other.m[15],	//yw
		m[8]*other.m[0]  + m[9]*other.m[4]  + m[10]*other.m[8]  + m[11]*other.m[12],	//zx
		m[8]*other.m[1]  + m[9]*other.m[5]  + m[10]*other.m[9]  + m[11]*other.m[13],	//zy
		m[8]*other.m[2]  + m[9]*other.m[6]  + m[10]*other.m[10] + m[11]*other.m[14],	//zz
		m[8]*other.m[3]  + m[9]*other.m[7]  + m[10]*other.m[11] + m[11]*other.m[15],	//zw
		m[12]*other.m[0] + m[13]*other.m[4] + m[14]*other.m[8]  + m[15]*other.m[12],	//wx
		m[12]*other.m[1] + m[13]*other.m[5] + m[14]*other.m[9]  + m[15]*other.m[13],	//wy
		m[12]*other.m[2] + m[13]*other.m[6] + m[14]*other.m[10] + m[15]*other.m[14],	//wz
		m[12]*other.m[3] + m[13]*other.m[7] + m[14]*other.m[11] + m[15]*other.m[15]	//ww
		);
}

afMat4	afMat4::operator*(const float other){
	return afMat4( 
		m[0] + other,
		m[1] + other,
		m[2] + other,
		m[3] + other,
		m[4] + other,
		m[5] + other,
		m[6] + other,
		m[7] + other,
		m[8] + other,
		m[9] + other,
		m[10] + other,
		m[11] + other,
		m[12] + other,
		m[13] + other,
		m[14] + other,
		m[15] + other);
}

float& afMat4::operator[](const int other){
	return m[other];
}

mat4& afMat4::operator*=(const mat4& other) {
	afMat4 temp = (*this)*other;
	*this = temp;
	return *this;
}
