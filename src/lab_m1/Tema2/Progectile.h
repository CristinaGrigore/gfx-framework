#pragma once

#include "components/simple_scene.h"
#include "CollisionObject.h"
#include <vector>
#include <iostream>

using namespace std;
 class Progectile : CollisionObject
    {
     public:
         Progectile();
        ~Progectile();
        Progectile(float x, float y, float z, float s, WindowObject* window);
        float pos_x, pos_y, pos_z, scale;
        float size;
        void SetShader(Shader *s);
        Shader* getShader();
        Mesh* getMesh();
   
     private:
        

     protected:
         Shader* shader;
         Mesh* mesh;
    };

