#include "lab_m1/Tema2/HUD.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

HUD::HUD() {
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
	scale_x =1;
    scale_y = 0.3f;

}
HUD::HUD(float x, float y, float z, float s_x, float s_y, WindowObject* window) {
    pos_x = x; pos_y = y; pos_z = z;
    scale_x = s_x;
    scale_y = s_y;

	{
        mesh = new Mesh("hud");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
	}

}

HUD::~HUD()
{

}

Mesh* HUD::getMesh() {
    return mesh;
}