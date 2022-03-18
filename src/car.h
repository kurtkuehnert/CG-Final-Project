#pragma once

#include "object.h"

#define MAX_SPEED_FORWARD 30.0f
#define MAX_SPEED_BACKWARD 5.0f
#define MAX_STEERING_ANGLE 30.0f
#define WHEEL_CIRCUMFERENCE M_PIf32
#define AXEL_DISTANCE 2.312324f
#define FRICTION 0.995f
#define STEERING_FRICTION 0.95f
#define ACCELERATION 0.1f

class Car {
public:
    Car();

    explicit Car(Object *model);

    void update(float direction, float steering);

    void draw();

private:
    vec3 _position = {0.0f, 0.0f, 0.0f};
    float _speed = 0.0f;
    float _rotation_angle = 0.0f;
    float _steering_angle = 0.0f;
    float _wheel_rotation = 0.0f;
    Object *_model;
};
