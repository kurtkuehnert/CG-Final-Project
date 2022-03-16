#include <cmath>
#include "planet.h"
#include "planetSimulation.h"

Planet::Planet(float radius, float mass, vec3 position, vec3 velocity, std::vector<Planet> planets,
               cg_object3D &model) {
    this->radius = radius;
    this->mass = mass;
    this->position = position;
    this->velocity = velocity;
    this->planets = planets;
    this->model = model;
}

void Planet::updatePos(float deltaTime) {
    // Beschleunigung berechnen
    vec3 accel = vec3(0, 0, 0);
    for (Planet pla: planets) {
        vec3 dir = pla.getPos() - position;
        float dist2 = pow(length(dir), 2);
        normalize(dir);
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
    glTranslatef(position.x, position.y, position.z);
    model.draw();
    glPopMatrix();
}

float Planet::getMass() {
    return mass;
}

vec3 Planet::getPos() {
    return position;
}