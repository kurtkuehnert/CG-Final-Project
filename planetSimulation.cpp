#include "planetSimulation.h"

PlanetSimulation::PlanetSimulation(cg_object3D objects[]) {
	PlanetSimulation(10, 7, objects);
}

PlanetSimulation::PlanetSimulation(int ammountSolarSystems, float averageAmmountPlanets, cg_object3D objects[]) {
	Planet sun = Planet(1, 100, CVector(0, 0, 0), CVector(0, 0, 0), std::vector<Planet>(), objects[TRUCK_TOP]);
	//Planet testPla = Planet(1, 1, CVector(5, 0, 0), CVector(0, 0, 5), std::vector<Planet>());
}

void PlanetSimulation::render() {
	for each (Planet pla in planets) {
		pla.render();
	}
}

void PlanetSimulation::update(float deltaTime) {
	for each (Planet pla in planets) {
		pla.updateVel(deltaTime);
	}

	for each (Planet pla in planets) {
		pla.updatePos(deltaTime);
	}
}