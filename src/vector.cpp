#include "vector.h"

#include <cstring> /* for memcpy() */
#include <iostream>
#include <cmath>

Vec3::Vec3() {
    zero();
}

Vec3::Vec3(float x, float y, float z) {
    set(x, y, z);
}

Vec3::Vec3(const float *a) {
    set(a);
}

float *Vec3::get_floats() {
    return _v;
}

float Vec3::x() const {
    return _v[0];
}

float Vec3::y() const {
    return _v[1];
}

float Vec3::z() const {
    return _v[2];
}

void Vec3::set(const float *a) {
    _v[0] = a[0];
    _v[1] = a[1];
    _v[2] = a[2];
}

void Vec3::set(float x, float y, float z) {
    _v[0] = x;
    _v[1] = y;
    _v[2] = z;
}

void Vec3::zero() {
    _v[0] = 0.0f;
    _v[1] = 0.0f;
    _v[2] = 0.0f;
}

Vec3 Vec3::operator*(float s) const {
    Vec3 t;
    t._v[0] = _v[0] * s;
    t._v[1] = _v[1] * s;
    t._v[2] = _v[2] * s;

    return t;
}

Vec3 Vec3::operator+(const Vec3 &b) const {
    Vec3 t;
    t._v[0] = _v[0] + b._v[0];
    t._v[1] = _v[1] + b._v[1];
    t._v[2] = _v[2] + b._v[2];
    return t;
}

Vec3 Vec3::operator-(const Vec3 &b) const {
    Vec3 t;
    t._v[0] = _v[0] - b._v[0];
    t._v[1] = _v[1] - b._v[1];
    t._v[2] = _v[2] - b._v[2];
    return t;
}

Vec3 Vec3::operator*(const Vec3 &b) const {
    Vec3 t;
    t._v[0] = _v[1] * b._v[2] - _v[2] * b._v[1];
    t._v[1] = _v[2] * b._v[0] - _v[0] * b._v[2];
    t._v[2] = _v[0] * b._v[1] - _v[1] * b._v[0];
    return t;
}

float Vec3::len_sqr() const {
    return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2];
}

float Vec3::len() const {
    return sqrtf(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
}

void Vec3::normalize() {
    float l = len();
    if (l != 0.0f) {
        l = 1.0f / l;
        _v[0] *= l;
        _v[1] *= l;
        _v[2] *= l;
    }
}

float Vec3::dot3(const Vec3 &b) const {
    return _v[0] * b._v[0] + _v[1] * b._v[1] + _v[2] * b._v[2];
}

Vec3 Vec3::rotateY(const float rot) const {
    Vec3 v(_v[0] * cos(rot) + _v[2] * sin(rot),
           0,
           _v[0] * -sin(rot) + _v[2] * cos(rot));
    return v;
}

void Vec3::print(const char *prefix) const {
    if (prefix == NULL) {
        prefix = "";
    }

    std::cout << prefix << "[" << _v[0] << " " << _v[1] << " " << _v[2] << "] (len: " << len() << ")" << std::endl;
}

