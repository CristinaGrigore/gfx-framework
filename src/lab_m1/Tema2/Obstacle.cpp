#include "lab_m1/Tema2/Obstacle.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Obstacle::Obstacle() {
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
	scale_x =1;
	scale_z = 1;

}
Obstacle::Obstacle(float x, float y, float z, float s1, float s2, WindowObject* window) {
    pos_x = x; pos_y = y; pos_z = z;
    scale_x = s1;
	scale_z = s2;

	{
		glm::vec3 baby_pink = glm::vec3(1, 0.7529f, 0.796f);
		glm::vec3 light_pink = glm::vec3(1, 0.4117f, 0.7058f);
		glm::vec3 hot_pink = glm::vec3(1, 0.0783f, 0.576f);
		vertices = vector<VertexFormat>
		{
			VertexFormat(glm::vec3(0, 0, 0),baby_pink),
			VertexFormat(glm::vec3(1, 0, 0), light_pink),
			VertexFormat(glm::vec3(1, 1, 0), baby_pink),
			VertexFormat(glm::vec3(0, 1, 0), light_pink),
			VertexFormat(glm::vec3(0, 0, 1), baby_pink),
			VertexFormat(glm::vec3(1, 0, 1), hot_pink),
			VertexFormat(glm::vec3(1, 1, 1), baby_pink),
			VertexFormat(glm::vec3(0, 1, 1), hot_pink)
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

Obstacle::~Obstacle()
{

}
void Obstacle::SetShader(Shader* s) {
    shader = s;
}
Shader* Obstacle::getShader() {
    return shader;
}

vector<unsigned int> Obstacle::getIndices() {
	return indices;
}
vector<VertexFormat> Obstacle::getVertices() {
	return vertices;
}