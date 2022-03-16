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
#include "wavefront.h"

#include <vector>

class Planet
{
public:
	Planet(float radius, float mass, CVector position, CVector velocity, std::vector<Planet> planets, cg_object3D& model);
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

