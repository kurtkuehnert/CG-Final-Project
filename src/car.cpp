#include "car.h"

#include <cmath>

#include "light.h"
#include "help.h"

Car::Car() : _model(nullptr) {}

Car::Car(Object *model) : _model(model) {}

void Car::update(float direction, float steering) {
    _speed += direction * ACCELERATION;
    _steering_angle += steering;

    if (_speed > MAX_SPEED_FORWARD) _speed = MAX_SPEED_FORWARD;
    if (_speed < -MAX_SPEED_BACKWARD) _speed = -MAX_SPEED_BACKWARD;
    if (_steering_angle > MAX_STEERING_ANGLE) _steering_angle = MAX_STEERING_ANGLE;
    if (_steering_angle < -MAX_STEERING_ANGLE) _steering_angle = -MAX_STEERING_ANGLE;

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

    _model->draw("body");
    _model->draw("door_left");
    _model->draw("door_right");
    _model->draw("interior");
    _model->draw("door_window_left");
    _model->draw("door_window_right");
    _model->draw("windows");

    // Help::drawCoordSystem(-2, 2, -2, 2, -2, 2);

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

    glPopMatrix();
}




