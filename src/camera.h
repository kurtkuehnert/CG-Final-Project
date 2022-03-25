#pragma once

#include <cmath>

#include "vector.h"
#include "window.h"
#include "input.h"

#define CAMERA_ROTATION_SPEED 0.5f
#define CAMERA_ZOOM_SPEED 0.2f

enum CameraMode {
    Observer,
    ThirdPerson,
    FirstPerson
};

class Camera {
public:
    void setMode(CameraMode mode);

    void update(Vec3 targetPosition, float targetRotation);

private:
    CameraMode _mode = Observer;
    float _radius = 8.0f;
    float _cameraAngle[2] = {};
};


