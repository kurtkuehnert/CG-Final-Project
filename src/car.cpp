#include "car.h"
#include "menu.h"

#include <cmath>
#include <algorithm>

Car::Car(Object *model) : _model(model) {
    _headlights[0] = new Light(1);
    _headlights[0]->setPosition(-0.8, 0.8, 1.8, 1);
    _headlights[0]->setDiffuse(100, 100, 80, 100);
    _headlights[0]->setSpotlight(0, 0, 1, 45, 1);
    _headlights[0]->setAttentuation(0, 1, 1);

    _headlights[1] = new Light(2);
    _headlights[1]->setPosition(0.8, 0.8, 1.8, 1);
    _headlights[1]->setDiffuse(100, 100, 80, 100);
    _headlights[1]->setSpotlight(0, 0, 1, 45, 1);
    _headlights[1]->setAttentuation(0, 1, 1);

    _break_lights[0] = new Light(3);
    _break_lights[0]->setPosition(-0.85, 0.8, -1.9, 1);
    _break_lights[0]->setDiffuse(10, 1, 1, 1);
    _break_lights[0]->setAttentuation(0, 1, 1);

    _break_lights[1] = new Light(4);
    _break_lights[1]->setPosition(0.85, 0.8, -1.9, 1);
    _break_lights[1]->setDiffuse(10, 1, 1, 1);
    _break_lights[1]->setAttentuation(0, 1, 1);
}

Car::~Car() {
    delete _headlights[0];
    delete _headlights[1];
    delete _break_lights[0];
    delete _break_lights[1];
}

void Car::update(float direction, float steering) {
    _speed += direction * ACCELERATION;
    _steering_angle += steering;
    _door_left_angle -= _door_left_open ? DOOR_SPEED : -DOOR_SPEED;
    _door_right_angle += _door_right_open ? DOOR_SPEED : -DOOR_SPEED;

    _speed = clamp(_speed, -MAX_SPEED_BACKWARD, MAX_SPEED_FORWARD);
    _steering_angle = clamp(_steering_angle, -MAX_STEERING_ANGLE, MAX_STEERING_ANGLE);
    _door_left_angle = clamp(_door_left_angle, -MAX_DOOR_ANGLE, 0.0f);
    _door_right_angle = clamp(_door_right_angle, 0.0f, MAX_DOOR_ANGLE);

    float distance = _speed * Menu::getDelta();

    _wheel_rotation += distance * 360.0f / WHEEL_CIRCUMFERENCE;

    float old_angle = _rotation_angle * M_PIf32 / 180.0f;
    Vec3 old_direction(sinf(old_angle), 0, cosf(old_angle));
    Vec3 old_front_point = old_direction * AXEL_DISTANCE;

    float new_angle = (_rotation_angle + _steering_angle) * M_PIf32 / 180.0f;;
    Vec3 new_direction(sin(new_angle), 0, cos(new_angle));
    Vec3 new_front_point = old_front_point + new_direction * distance;

    Vec3 current_direction = new_front_point;
    current_direction.normalize();

    _position = _position + new_front_point - current_direction * AXEL_DISTANCE;
    _rotation_angle = asinf(current_direction.x()) * 180.0f / M_PIf32;
    if (current_direction.z() < 0) _rotation_angle = 180.0f - _rotation_angle;

    _speed *= FRICTION;

    if (fabs(_speed) > 0.1f) _steering_angle *= STEERING_FRICTION;

    _break_lights[0]->enabled = direction < 0;
    _break_lights[1]->enabled = direction < 0;
}

void Car::lights() {
    glPushMatrix();
    glTranslatef(_position.x(), 0, _position.z());
    glRotatef(_rotation_angle, 0, 1, 0);
    glTranslatef(0.0f, 0.0f, 1.240854f);

    _headlights[0]->draw();
    _headlights[1]->draw();

    _break_lights[0]->draw();
    _break_lights[1]->draw();

    glPopMatrix();
}


void Car::draw() {
    glPushMatrix();
    glTranslatef(_position.x(), 0, _position.z());
    glRotatef(_rotation_angle, 0, 1, 0);
    glTranslatef(0.0f, 0.0f, 1.240854f);

    //     Menu::drawCoordSystem(-2, 2, -2, 2, -2, 2);

    _model->draw("body");
    _model->draw("interior");

    glPushMatrix();
    glTranslatef(0.0f, 0.324157f, -1.240854f);
    glRotatef(_wheel_rotation, 1, 0, 0);
    glTranslatef(0.0f, -0.324157f, 1.240854f);
    _model->draw("wheel_back_left");
    _model->draw("wheel_back_right");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.652940f, 0.324157f, 1.07147f);
    glRotatef(_steering_angle, 0, 1, 0);
    glRotatef(_wheel_rotation, 1, 0, 0);
    glTranslatef(-0.652940f, -0.324157f, -1.07147f);
    _model->draw("wheel_front_left");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.652940f, 0.324157f, 1.07147f);
    glRotatef(_steering_angle, 0, 1, 0);
    glRotatef(_wheel_rotation, 1, 0, 0);
    glTranslatef(0.652940f, -0.324157f, -1.07147f);
    _model->draw("wheel_front_right");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.839348f, 0.0f, 0.650257f);
    glRotatef(_door_left_angle, 0, 1, 0);
    glTranslatef(-0.839348f, 0.0f, -0.650257f);
    _model->draw("door_left");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.839348f, 0.0f, 0.650257f);
    glRotatef(_door_right_angle, 0, 1, 0);
    glTranslatef(0.839348f, 0.0f, -0.650257f);
    _model->draw("door_right");
    glPopMatrix();

    _model->draw("windows");

    glPushMatrix();
    glTranslatef(0.839348f, 0.0f, 0.650257f);
    glRotatef(_door_left_angle, 0, 1, 0);
    glTranslatef(-0.839348f, 0.0f, -0.650257f);
    _model->draw("door_window_left");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.839348f, 0.0f, 0.650257f);
    glRotatef(_door_right_angle, 0, 1, 0);
    glTranslatef(0.839348f, 0.0f, -0.650257f);
    _model->draw("door_window_right");
    glPopMatrix();

    glPopMatrix();
}

void Car::toggleLeftDoor() {
    _door_left_open = !_door_left_open;
}

void Car::toggleRightDoor() {
    _door_right_open = !_door_right_open;
}

Vec3 Car::getPosition() {
    return _position + Vec3(0, 1.3f, 0);
}

float Car::getRotation() {
    return _rotation_angle;
}

void Car::toggleHeadlights() {
    _headlights[0]->toggle();
    _headlights[1]->toggle();
}


