#include "camera.h"

#include <algorithm>

void Camera::setMode(CameraMode mode) {
    _mode = mode;
}

void Camera::update(Vec3 target_position, float target_rotation) {
    if (Mouse::buttonState(GLUT_LEFT_BUTTON)) {
        _cameraAngle[0] +=
            CAMERA_ROTATION_SPEED * static_cast<float>(Mouse::moveX()) / GlobalState::screenSize[0] * M_PIf32;
        _cameraAngle[1] +=
            CAMERA_ROTATION_SPEED * static_cast<float>(Mouse::moveY()) / GlobalState::screenSize[1] * M_PIf32;
    }
    if (Mouse::buttonState(GLUT_MIDDLE_BUTTON)) {
        _radius += CAMERA_ZOOM_SPEED * static_cast<float>(Mouse::moveY());
        _radius = clamp(_radius, 1.0f, 30.0f);
    }

    if (_mode == Observer) {
        gluLookAt(0, _radius, 0, target_position.x(), target_position.y(), target_position.z(), 0, 1, 0);
    } else if (_mode == ThirdPerson) {
        float Phi = _cameraAngle[0] - M_PIf32 * 0.5f - target_rotation * M_PIf32 / 180.0f;
        float The = _cameraAngle[1] + M_PIf32 * 0.125f;
        int up = (The <= 0.5 * M_PI || The > 1.5 * M_PI) * 2 - 1;

        Vec3 cameraPosition = target_position + Vec3(cos(Phi) * cos(The), sin(The), sin(Phi) * cos(The)) * _radius;

        gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(), target_position.x(), target_position.y(),
                  target_position.z(), 0, up, 0);
    } else if (_mode == FirstPerson) {
        Vec3 cameraPosition = target_position + Vec3(0.4f, 0, 0.85f).rotateY(target_rotation * M_PIf32 / 180.0f);
        target_position =
            cameraPosition +
            Vec3(0.0f, 0.0f, -1.0f).rotateY(_cameraAngle[0] + M_PIf32 + target_rotation * M_PIf32 / 180.0f);

        gluLookAt(cameraPosition.x(), cameraPosition.y(), cameraPosition.z(), target_position.x(), target_position.y(),
                  target_position.z(), 0, 1, 0);
    }
}


