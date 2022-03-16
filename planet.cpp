#include "planet.h"
#include "planetSimulation.h"

Planet::Planet(float radius, float mass, CVector position, CVector velocity, std::vector<Planet> planets, cg_object3D& model) {
	this->radius = radius;
	this->mass = mass;
	this->position = position;
	this->velocity = velocity;
	this->planets = planets;
	this->model = model;
}

void Planet::updatePos(float deltaTime) {
	// Beschleunigung berechnen
	CVector accel = CVector(0, 0, 0);
	for each (Planet pla in planets) {
		CVector dir = pla.getPos() - position;
		float dist2 = pow(dir.len(), 2);
		dir.normalize();
		accel = accel + (dir * G * (pla.getMass() / dist2));
	}

	// Neue Geschwindigkeit
	velocity = velocity + accel * deltaTime;
}

void Planet::updateVel(float deltaTime) {
	position = position + velocity * deltaTime;
}

void Planet::render() {
	glPushMatrix();
	glTranslatef(position.x(), position.y(), position.z());
	model.draw();
	glPopMatrix();
}

float Planet::getMass() {
	return mass;
}

CVector Planet::getPos() {
	return position;
}