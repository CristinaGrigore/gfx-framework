#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>

using namespace std;
 class HUD
    {
     public:
         HUD();
        ~HUD();
        HUD(float x, float y, float z, float s_x, float s_y, WindowObject* window);
        float pos_x, pos_y, pos_z, scale_x = 2.f, scale_y = 0.3f;
        Mesh* getMesh();
        float offset_x = 0.3f, offset_y = 1.2f, offset_z = 0.f;
       
     private:
        

     protected:
       
         Mesh* mesh;
    };

