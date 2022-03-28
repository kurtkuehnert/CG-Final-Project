#pragma once

#include "../Include/lib.h"

void setLights();

class Light {
public:
    explicit Light(int num);

    void setPosition(float x, float y, float z, float w);

    void setRotation(float horizontal, float vertikal);

    void setAmbient(float r, float g, float b, float a);

    void setDiffuse(float r, float g, float b, float a);

    void setSpecular(float r, float g, float b, float a);

    void setSpotlight(float directionX, float directionY, float directionZ, float cutoff, float exponent);

    void setAttentuation(float constant, float linear, float quadric);

    void draw();

    void toggle();

    void markLightPosition();

    float pos[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float rot[2] = {0.0f, 0.0f};
    float spot[5] = {0.0f, -1.0f, 0.0f, 180.0f, 0.0f};

    bool enabled = true;
private:
    int _id;

    float _amb[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float _diff[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float _spec[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float _att[3] = {1.0f, 0.0f, 0.0f};
};
