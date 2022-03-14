#include "vector.h"

#include <cstring> /* for memcpy() */
#include <iostream>

CVector::CVector()
{
	zero();
}

CVector::CVector(float x, float y, float z)
{
	set(x,y,z);
}

CVector::CVector(const float *a)
{
	set(a);
}

float * CVector::get_floats()
{
	return _v;
}

float CVector::x() const
{
	return _v[0];
}

float CVector::y() const
{
	return _v[1];
}

float CVector::z() const
{
	return _v[2];
}

void CVector::set(const float *a)
{
	_v[0]=a[0];
	_v[1]=a[1];
	_v[2]=a[2];
}

void CVector::set(float x, float y, float z)
{
	_v[0]=x;
	_v[1]=y;
	_v[2]=z;
}

void CVector::zero()
{
	_v[0]=0.0f;
	_v[1]=0.0f;
	_v[2]=0.0f;
}

CVector CVector::operator*(float s) const
{
	CVector t;
	t._v[0]=_v[0] * s;
	t._v[1]=_v[1] * s;
	t._v[2]=_v[2] * s;

	return t;
}

CVector CVector::operator+(const CVector& b) const
{
	CVector t;
	t._v[0]=_v[0] + b._v[0];
	t._v[1]=_v[1] + b._v[1];
	t._v[2]=_v[2] + b._v[2];
	return t;
}

CVector CVector::operator-(const CVector& b) const
{
	CVector t;
	t._v[0]=_v[0] - b._v[0];
	t._v[1]=_v[1] - b._v[1];
	t._v[2]=_v[2] - b._v[2];
	return t;
}

CVector CVector::operator*(const CVector& b) const
{
	CVector t;
	t._v[0] = _v[1]*b._v[2] - _v[2]*b._v[1];
	t._v[1] = _v[2]*b._v[0] - _v[0]*b._v[2];
	t._v[2] = _v[0]*b._v[1] - _v[1]*b._v[0];
	return t;
}
	
float CVector::len_sqr() const
{
	return _v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2];
}

float CVector::len() const
{
	return sqrtf(_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2]);
}

void CVector::normalize()
{
	float l=len();
	if (l != 0.0f) {
		l = 1.0f / l;
		_v[0] *= l;
		_v[1] *= l;
		_v[2] *= l;
	}
}

float CVector::dot3(const CVector &b) const
{
	return _v[0]*b._v[0] + _v[1]*b._v[1] + _v[2]*b._v[2];
}

CVector CVector::rotateY(const float rot) const
{
	CVector v(	_v[0] * cos(rot) + _v[2] * sin(rot), 
				0, 
				_v[0] * -sin(rot) + _v[2] * cos(rot));
	return v;
}

void CVector::print(const char *prefix) const
{
	if (prefix == NULL) {
		prefix="";
	}
	
	std::cout << prefix << "[" << _v[0] << " " << _v[1] << " " << _v[2] << "] (len: " << len() << ")" << std::endl;
}

