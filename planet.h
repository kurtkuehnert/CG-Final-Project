#pragma once

#include <vector>

#include "Include/lib.h"
#include "object.h"


class Planet {
public:
    Planet(float radius, float mass, vec3 position, vec3 velocity, vector<Planet> planets, Object &model);

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
    Object model;
};

