#pragma once

#include <vector>

#include "Include/lib.h"
#include "wavefront.h"


class Planet {
public:
    Planet(float radius, float mass, vec3 position, vec3 velocity, vector<Planet> planets, cg_object3D &model);

    void updateVel(float deltaTime);

    void updatePos(float deltaTime);

    void render();

    float getMass();

    vec3 getPos();

private:
    float radius;
    float mass;
    vec3 position;
    vec3 velocity;
    std::vector<Planet> planets;
    cg_object3D model;
};

