#pragma once

#include "Include/lib.h"
#include "planet.h"
#include "assets.h"
#include <vector>

static const float G = 0.0000000000674f;

class PlanetSimulation {
public:

    explicit PlanetSimulation(Object *objects);

    PlanetSimulation(int ammountSolarSystems, float averageAmmountPlanets, Object *objects);

    void render();

    void update(float deltaTime);

private:
    std::vector<Planet> planets;
};

