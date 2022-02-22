#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "components/simple_scene.h"

class GameObject
{
public:
    // object state
    glm::vec2   pos, size, velocity;
    glm::vec3   color;
    float       rot;
    bool        solid;
    bool        destroyed;
    // constructors
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

};

#endif