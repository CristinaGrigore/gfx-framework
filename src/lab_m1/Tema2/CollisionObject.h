#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>

using namespace std;

 class CollisionObject
    {
     public:
         CollisionObject();
        ~CollisionObject();
        glm::vec3 GetTransform();
        glm::vec3 GetRotation();
        glm::vec3 GetScale();

        void SetTransform(glm::vec3);
        void SetRotation(glm::vec3);
        void SetScale(glm::vec3);

        enum Direction {
            UP,
            RIGHT,
            DOWN,
            LEFT
        };

        struct BoundingBox
        {
            float maxX, minX;
            float maxY, minY;
            float maxZ, minZ;
        };

        bool CheckCollision(CollisionObject* o2);
        BoundingBox getBoundingBox();
        void SetBoundingBox(float, float, float, float, float, float);

     private:
         BoundingBox box;
         glm::vec3 transform;
         glm::vec3 rotation;
         glm::vec3 scale;

     protected:
        
    };

