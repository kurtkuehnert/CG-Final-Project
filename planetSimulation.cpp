#include "planetSimulation.h"

PlanetSimulation::PlanetSimulation(Object *objects) : PlanetSimulation(10, 7, objects) {}

PlanetSimulation::PlanetSimulation(int ammountSolarSystems, float averageAmmountPlanets, Object *objects) {
    // Planet sun = Planet(1, 100, CVector(0, 0, 0), CVector(0, 0, 0), std::vector<Planet>(), objects[TRUCK_TOP]);
    //Planet testPla = Planet(1, 1, CVector(5, 0, 0), CVector(0, 0, 5), std::vector<Planet>());
}

void PlanetSimulation::render() {
    for (Planet planet: planets) {
        planet.render();
    }
}

void PlanetSimulation::update(float deltaTime) {
    for (Planet planet: planets) {
        planet.updateVel(deltaTime);
    }

    for (Planet planet: planets) {
        planet.updatePos(deltaTime);
    }
}