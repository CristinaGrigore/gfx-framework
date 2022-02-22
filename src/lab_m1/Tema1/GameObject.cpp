#include "GameObject.h"

GameObject::GameObject()
    : pos(0.0f, 0.0f), size(0.0f, 0.0f), velocity(0.0f), color(1.0f), rot(0.0f), solid(false), destroyed(false) { }

GameObject::GameObject(glm::vec2 p, glm::vec2 s, glm::vec3 c, glm::vec2 v)
    : pos(p), size(s), velocity(v), color(c), rot(0.0f), solid(false), destroyed(false) { }

