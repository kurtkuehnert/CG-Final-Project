#pragma once

#include "object.h"
#include "vector.h"

#define MAX_SPEED_FORWARD 30.0f
#define MAX_SPEED_BACKWARD 5.0f
#define MAX_STEERING_ANGLE 30.0f
#define WHEEL_CIRCUMFERENCE M_PIf32
#define AXEL_DISTANCE 2.312324f
#define FRICTION 0.995f
#define STEERING_FRICTION 0.95f
#define ACCELERATION 0.1f
#define MAX_DOOR_ANGLE 60.0f
#define DOOR_SPEED 1.0f

class Car {
public:
    explicit Car(Object *model);

    void update(float direction, float steering);

    void toggleLeftDoor();

    void toggleRightDoor();

    void draw();

    Vec3 getPosition();

    float getRotation();

private:
    Vec3 _position = {0.0f, 0.0f, 3.0f};
    float _speed = 0.0f;
    float _rotation_angle = 0.0f;
    float _steering_angle = 0.0f;
    float _wheel_rotation = 0.0f;
    float _door_left_angle = 0.0f;
    float _door_right_angle = 0.0f;
    bool _door_left_open = false;
    bool _door_right_open = false;
    Object *_model;
};
