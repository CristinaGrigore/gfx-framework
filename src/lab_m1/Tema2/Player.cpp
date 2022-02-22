#include "lab_m1/Tema2/Player.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
Player::Player() {}

Player::Player(float x, float y, float z, WindowObject* window)
{
	collided = false;
	destroyed = false;
	attack = false;
	angular_velocity = glm::vec3(0, 0, 0);
	direction = glm::vec3(1, 0, 0);
	lives = 1000;
	forwards = backwards = left = right = true;
	velocity = 5.f;
   
    head_size = 10; 
    scale_head = 0.2;
	scale_body_x = 0.3; scale_body_y = 0.6, scale_body_z = 0.2;
	scale_leg_x = 0.14; scale_leg_y = 0.4; scale_leg_z = 0.08;

	offset_body_x = -0.074f;
	offset_body_y = -0.7;
	offset_body_z = 0.f;

	offset_hand_right_x = -0.09;
	offset_hand_right_y = 0.f;
	offset_hand_right_z = 0.02f;

	offset_hand_left_x = scale_body_x;
	offset_hand_left_y = 0.f;
	offset_hand_left_z = 0.02f;

	offset_leg_right_x = -0.02f;
	offset_leg_right_y = -0.5f;
	offset_leg_right_z = +0.02f;

	offset_leg_left_x = scale_leg_x + 0.02f;
	offset_leg_left_y = -0.5f;
	offset_leg_left_z = 0.02f;

	pos_head_x = x; pos_head_y = y; pos_head_z = z;

    pos_body_x = pos_head_x + offset_body_x;
	pos_body_y = pos_head_y + offset_body_y;
	pos_body_z = pos_head_z + offset_body_z;

    pos_hand_right_x = pos_body_x + offset_hand_right_x;
	pos_hand_right_y = pos_body_y + offset_hand_right_y;
	pos_hand_right_z = pos_body_z + offset_hand_right_z;

    pos_hand_left_x = pos_body_x + offset_hand_left_x;
	pos_hand_left_y = pos_body_y + offset_hand_right_y;
	pos_hand_left_z = pos_body_z + offset_hand_right_z;

    pos_leg_right_x = pos_body_x + offset_leg_right_x;
	pos_leg_right_y = pos_body_y + offset_leg_right_y;
	pos_leg_right_z = pos_body_z + offset_leg_right_z;

    pos_leg_left_x = pos_body_x + offset_leg_left_x;
	pos_leg_left_y = pos_body_y + offset_leg_left_y;
	pos_leg_left_z = pos_body_z  + offset_leg_left_z;

	// Create a mesh box using custom data
	{
		glm::vec3 baby_pink = glm::vec3(1, 0.7529f, 0.796f);
		glm::vec3 light_pink = glm::vec3(1, 0.4117f, 0.7058f);
		vertices = vector<VertexFormat>
		{
			VertexFormat(glm::vec3(0, 0, 0), light_pink),
			VertexFormat(glm::vec3(1, 0, 0),baby_pink),
			VertexFormat(glm::vec3(1, 1, 0), baby_pink),
			VertexFormat(glm::vec3(0, 1, 0), light_pink),
			VertexFormat(glm::vec3(0, 0, 1),baby_pink),
			VertexFormat(glm::vec3(1, 0, 1), baby_pink),
			VertexFormat(glm::vec3(1, 1, 1), light_pink),
			VertexFormat(glm::vec3(0, 1, 1), baby_pink)
		};
		
		indices = vector<unsigned int>
		{
			0, 5, 1,	
			0, 4, 5,	
			0, 7, 4,	
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


Player::~Player()
{
}
void Player::SetShader(Shader* s) {
    shader = s;
}
void Player::Update(float x, float y, float z) {
	pos_head_x = x; pos_head_y = y; pos_head_z = z;

	pos_body_x = pos_head_x + offset_body_x;
	pos_body_y = pos_head_y + offset_body_y;
	pos_body_z = pos_head_z + offset_body_z;

	pos_hand_right_x = pos_body_x + offset_hand_right_x;
	pos_hand_right_y = pos_body_y + offset_hand_right_y;
	pos_hand_right_z = pos_body_z + offset_hand_right_z;

	pos_hand_left_x = pos_body_x + offset_hand_left_x;
	pos_hand_left_y = pos_body_y + offset_hand_left_y;
	pos_hand_left_z = pos_body_z + offset_hand_left_z;

	pos_leg_right_x = pos_body_x + offset_leg_right_x;
	pos_leg_right_y = pos_body_y + offset_leg_right_y;
	pos_leg_right_z = pos_body_z + offset_leg_right_z;

	pos_leg_left_x = pos_body_x + offset_leg_left_x;
	pos_leg_left_y = pos_body_y + offset_leg_left_y;
	pos_leg_left_z = pos_body_z + offset_leg_left_z;
	

}
Shader* Player::GetShader() {
    return shader;
}
Texture2D* Player::GetTexture() {
    return texture;
}
vector<unsigned int> Player::getIndices() {
    return indices;
}
vector<VertexFormat> Player::getVertices() {
    return vertices;
}
void Player::OnCollision(CollisionObject::Direction lastDirection) {
	collided = true;
	//printf("Collision happened on dir %d\n", lastDirection);
	switch (lastDirection) {
		case UP:
			forwards = false;
			break;
		case DOWN:
			backwards = false;
			break;
		case LEFT:
			left = false;
		//	printf("NO LEFTTT\n");
			break;
		case RIGHT:
			right = false;
			break;
	}
		
}
void Player::resolveCollision(CollisionObject::Direction lastDirection) {
	//collided = false;
	//printf("Collision resolved on dir %d\n", lastDirection);
	switch (lastDirection) {
	case UP:
		forwards = true;
		break;
	case DOWN:
		backwards = true;
		break;
	case LEFT:
		left = true;
		break;
	case RIGHT:
		right = true;
		break;
	}

}

void Player::SetVelocity(float v) {
	velocity = v;
}

void Player::SetAngularVelocity(glm::vec3 v) {
	angular_velocity = v;
}

float Player::GetVelocity() {
	return velocity;
}

glm::vec3 Player::GetAngularVelocity() {
	return angular_velocity;
}