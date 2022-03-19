#include "car.h"

#include <cmath>

#include "light.h"
#include "help.h"

Car::Car() : _model(nullptr) {}

Car::Car(Object *model) : _model(model) {
    _model->getMesh("windows")->setOpacity(0.5f);
    _model->getMesh("door_window_left")->setOpacity(0.5f);
    _model->getMesh("door_window_right")->setOpacity(0.5f);
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

    float delta_time = 1.0f / 60.0f;
    float distance = _speed * delta_time;

    _wheel_rotation += distance * 360.0f / WHEEL_CIRCUMFERENCE;

    float old_angle = radians(_rotation_angle);
    vec3 old_direction(sinf(old_angle), 0, cosf(old_angle));
    vec3 old_front_point = old_direction * AXEL_DISTANCE;

    float new_angle = radians(_rotation_angle + _steering_angle);
    vec3 new_direction(sin(new_angle), 0, cos(new_angle));
    vec3 new_front_point = old_front_point + new_direction * distance;

    vec3 current_direction = normalize(new_front_point);

    _position += new_front_point - current_direction * AXEL_DISTANCE;
    _rotation_angle = degrees(asinf(current_direction.x));
    if (current_direction.z < 0) _rotation_angle = 180.0f - _rotation_angle;

    _speed *= FRICTION;

    if (fabs(_speed) > 0.1f) _steering_angle *= STEERING_FRICTION;
}

void Car::draw() {
    glPushMatrix();
    glTranslatef(_position.x, 0, _position.z);
    glRotatef(_rotation_angle, 0, 1, 0);
    glTranslatef(0.0f, 0.0f, 1.240854f);

    // Help::drawCoordSystem(-2, 2, -2, 2, -2, 2);

    _model->getMesh("body")->draw();
    _model->getMesh("interior")->draw();

    glPushMatrix();
    glTranslatef(0.0f, 0.324157f, -1.240854f);
    glRotatef(_wheel_rotation, 1, 0, 0);
    glTranslatef(0.0f, -0.324157f, 1.240854f);
    _model->getMesh("wheel_back_left")->draw();
    _model->getMesh("wheel_back_right")->draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.652940f, 0.324157f, 1.07147f);
    glRotatef(_steering_angle, 0, 1, 0);
    glRotatef(_wheel_rotation, 1, 0, 0);
    glTranslatef(-0.652940f, -0.324157f, -1.07147f);
    _model->getMesh("wheel_front_left")->draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.652940f, 0.324157f, 1.07147f);
    glRotatef(_steering_angle, 0, 1, 0);
    glRotatef(_wheel_rotation, 1, 0, 0);
    glTranslatef(0.652940f, -0.324157f, -1.07147f);
    _model->getMesh("wheel_front_right")->draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.839348f, 0.0f, 0.650257f);
    glRotatef(_door_left_angle, 0, 1, 0);
    glTranslatef(-0.839348f, 0.0f, -0.650257f);
    _model->getMesh("door_left")->draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.839348f, 0.0f, 0.650257f);
    glRotatef(_door_right_angle, 0, 1, 0);
    glTranslatef(0.839348f, 0.0f, -0.650257f);
    _model->getMesh("door_right")->draw();
    glPopMatrix();

    _model->getMesh("windows")->draw();

    glPushMatrix();
    glTranslatef(0.839348f, 0.0f, 0.650257f);
    glRotatef(_door_left_angle, 0, 1, 0);
    glTranslatef(-0.839348f, 0.0f, -0.650257f);
    _model->getMesh("door_window_left")->draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.839348f, 0.0f, 0.650257f);
    glRotatef(_door_right_angle, 0, 1, 0);
    glTranslatef(0.839348f, 0.0f, -0.650257f);
    _model->getMesh("door_window_right")->draw();
    glPopMatrix();

    glPopMatrix();
}

void Car::toggleLeftDoor() {
    _door_left_open = !_door_left_open;
}

void Car::toggleRightDoor() {
    _door_right_open = !_door_right_open;
}




