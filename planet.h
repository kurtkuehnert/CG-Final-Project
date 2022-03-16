#pragma once

#include <vector>

#include "Include/gl.h"
#include "vector.h"
#include "wavefront.h"


class Planet {
public:
    Planet(float radius, float mass, CVector position, CVector velocity, std::vector<Planet> planets,
           cg_object3D &model);

    void updateVel(float deltaTime);

    void updatePos(float deltaTime);

    void render();

    float getMass();

    CVector getPos();

private:
    float radius;
    float mass;
    CVector position;
    CVector velocity;
    std::vector<Planet> planets;
    cg_object3D model;
};

