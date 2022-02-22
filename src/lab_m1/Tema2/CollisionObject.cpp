#include "lab_m1/Tema2/CollisionObject.h"
#include <vector>
#include <string>
#include <iostream>


#define DISTANCE_LOOKUP 1

CollisionObject::CollisionObject() {
 

}
CollisionObject::~CollisionObject()
{

}
CollisionObject::Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }

    return (CollisionObject::Direction)best_match;
}
bool CollisionObject::CheckCollision( CollisionObject* o2) {
  
   // if (glm::distance(transform, o2->GetTransform()) > DISTANCE_LOOKUP)
    //    return false;
    BoundingBox a = box;
    BoundingBox b = o2->getBoundingBox();
    // get center point circle first 
    glm::vec2 center(a.maxX * 0.5f);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents((b.maxX - b.minX) / 2.0f, (b.maxZ - b.minZ) / 2.0f);
    glm::vec2 aabb_center(b.minX + aabb_half_extents.x, b.minZ + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // now retrieve vector between center circle and closest point AABB and check if length < radius
    
    bool col = (a.minX < b.maxX && a.maxX > b.minX) &&
        (a.minY < b.maxY && a.maxY > b.minY) &&
        (a.minZ < b.maxZ && a.maxZ > b.minZ);
    if (col) {
        difference = closest - center;
        CollisionObject::Direction dir = VectorDirection(difference);
        /*
        switch (dir) {
        case 0:
            printf("DIRECTION UP\n");
            break;
        case 1:
            printf("DIRECTION RIGHT\n");
            break;
        case 2:
            printf("DIRECTION DOWN\n");
            break;
        case 3:
            printf("DIRECTION LEFT\n");
            break;
        }
        */
    }
    return col;
}

glm::vec3 CollisionObject::GetTransform() {
    return transform;
}
glm::vec3 CollisionObject::GetRotation() {
    return rotation;
}
glm::vec3 CollisionObject::GetScale() {
    return scale;
}
void CollisionObject::SetTransform(glm::vec3 t) {
    transform = t;
}
void CollisionObject::SetRotation(glm::vec3 r) {
    rotation = r;
}
void CollisionObject::SetScale(glm::vec3 s) {
    scale = s;
}
CollisionObject::BoundingBox CollisionObject::getBoundingBox() {
    return box;
}

void CollisionObject::SetBoundingBox(float x, float X, float y, float Y, float z, float Z) {
    box.minX = x;   box.minY = y;   box.minZ = z;
    box.maxX = X;   box.maxY = Y;   box.maxZ = Z;
}

