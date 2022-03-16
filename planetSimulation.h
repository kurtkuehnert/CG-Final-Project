#pragma once

#include "Include/gl.h"
#include "vector.h"
#include "planet.h"
#include "assets.h"
#include <vector>

static const float G = 0.0000000000674f;

class PlanetSimulation {
public:

    explicit PlanetSimulation(cg_object3D *objects);

    PlanetSimulation(int ammountSolarSystems, float averageAmmountPlanets, cg_object3D *objects);

    void render();

    void update(float deltaTime);

private:
    std::vector<Planet> planets;
};

