#pragma once

#ifdef _WIN32
#include "Include/glew.h"
#include "Include/freeglut.h"
#elif __linux__
#include "GL/glew.h"
#include "GL/glut.h"
#elif __APPLE__
#include "Include/glew.h"
#include "Include/freeglut.h"
#endif

#include "vector.h"
#include "planet.h"
#include "main.h"
#include <vector>

static const float G = 0.0000000000674f;

class PlanetSimulation
{
public:
	

	PlanetSimulation(cg_object3D objects[]);
	PlanetSimulation(int ammountSolarSystems, float averageAmmountPlanets, cg_object3D objects[]);
	void render();
	void update(float deltaTime);

private:
	std::vector<Planet> planets;
};

