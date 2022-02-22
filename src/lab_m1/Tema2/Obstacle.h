#pragma once

#include "components/simple_scene.h"
#include "CollisionObject.h"
#include <vector>
#include <iostream>

using namespace std;
 class Obstacle : CollisionObject
    {
     public:
         Obstacle();
        ~Obstacle();
        Obstacle(float x, float y, float z, float s1, float s2, WindowObject* window);
        float pos_x, pos_y, pos_z, scale_x, scale_z;
        float size;
        void SetShader(Shader *s);
        Shader* getShader();
        vector<VertexFormat> getVertices();
        vector<unsigned int> getIndices();
     private:
        

     protected:
         Shader* shader;
         vector<VertexFormat> vertices;
         vector<unsigned int> indices;
    };

