#include "lab_m1/Tema2/Enemy.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Enemy::Enemy() {
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
	scale_x = 3;
	scale_z = 3;

	
}
Enemy::Enemy(float x, float y, float z, float s1, float s2, float s3, WindowObject* window) {
    pos_x = x; pos_y = y; pos_z = z;
    scale_x = s1;
	scale_z = s3;
	scale_y = s2;
	{
		vertices = vector<VertexFormat>
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(.5f, .5f, 1.f)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1.f, .5f, 1.f)),
			VertexFormat(glm::vec3(1, 1, 0), glm::vec3(.5f, 1.f, 1.f)),
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(.75f, .25f, .25f)),
			VertexFormat(glm::vec3(0, 0, 1), glm::vec3(1.f, .5f, .5f)),
			VertexFormat(glm::vec3(1, 0, 1), glm::vec3(.25f, .75f, .75f)),
			VertexFormat(glm::vec3(1, 1, 1), glm::vec3(1.f, 1.f, .5f)),
			VertexFormat(glm::vec3(0, 1, 1), glm::vec3(.5f, 1.f, .5f))
		};

		indices = vector<unsigned int>
		{
			0, 5, 1,	// indices for first triangle
			0, 4, 5,	// indices for second triangle
			0, 7, 4,	// etc.
			0, 3, 7,
			3, 6, 7,
			3, 2, 6,
			2, 5, 6,
			2, 1, 5,
			0, 1, 2,
			0, 2, 3,
			4, 6, 5,
			4, 7, 6
		};
	}

}

Enemy::~Enemy()
{

}
void Enemy::SetShader(Shader* s) {
    shader = s;
}
Shader* Enemy::getShader() {
    return shader;
}

vector<unsigned int> Enemy::getIndices() {
	return indices;
}
vector<VertexFormat> Enemy::getVertices() {
	return vertices;
}