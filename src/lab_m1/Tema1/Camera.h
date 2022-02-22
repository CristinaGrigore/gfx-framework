#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "components/simple_scene.h"

class Camera
{
public:
    // object state
    glm::vec2   pos;
    // constructors
    Camera();
    Camera(glm::vec2 pos);


};

#endif