#include "lab_m1/Tema2/Progectile.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Progectile::Progectile() {
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
	scale =1;

}
Progectile::Progectile(float x, float y, float z, float s, WindowObject* window) {
    pos_x = x; pos_y = y; pos_z = z;
    scale = s;

	{
        mesh = new Mesh("progectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
       // meshes[mesh->GetMeshID()] = mesh;
	}

}

Progectile::~Progectile()
{

}
void Progectile::SetShader(Shader* s) {
    shader = s;
}
Shader* Progectile::getShader() {
    return shader;
}

Mesh* Progectile::getMesh() {
    return mesh;
}