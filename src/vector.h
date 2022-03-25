#pragma once

class Vec3 {
public:
    Vec3();

    Vec3(float x, float y, float z);

    Vec3(const float *);

    void set(const float *a);

    void set(float x, float y, float z);

    void zero();

    float *get_floats();

    float x() const;

    float y() const;

    float z() const;

    float len() const;

    float len_sqr() const;

    // Operatoren
    Vec3 operator+(const Vec3 &b) const;

    Vec3 operator-(const Vec3 &b) const;

    Vec3 operator*(const float s) const;

    Vec3 operator*(const Vec3 &b) const;

    float dot3(const Vec3 &b) const;

    Vec3 rotateY(const float rot) const;

    void normalize();

    void print(const char *prefix = "") const;

private:
    float _v[3];
};
