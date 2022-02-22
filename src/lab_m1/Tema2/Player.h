#pragma once

#include "components/simple_scene.h"
#include "CollisionObject.h"
#include <vector>
#include <iostream>

using namespace std;
 class Player : CollisionObject
    {
     public:
        Player();
        Player(float, float, float, WindowObject* window);
        ~Player();
        
            float pos_head_x, pos_head_y, pos_head_z, head_size, scale_head;
            float pos_body_x, pos_body_y, pos_body_z, scale_body_x, scale_body_y, scale_body_z;
            float pos_hand_right_x, pos_hand_right_y, pos_hand_right_z;
            float pos_hand_left_x, pos_hand_left_y, pos_hand_left_z, scale_hand;
            float pos_leg_left_x, pos_leg_left_y, pos_leg_left_z, scale_leg_x, scale_leg_y, scale_leg_z;
            float pos_leg_right_x, pos_leg_right_y, pos_leg_right_z;
            void SetShader(Shader* s);
            Shader* GetShader();
            Texture2D* GetTexture();
            vector<VertexFormat> getVertices();
            vector<unsigned int> getIndices();
            void OnCollision(CollisionObject::Direction d);
            void resolveCollision(CollisionObject::Direction d);
            void SetVelocity(float);
            void SetAngularVelocity(glm::vec3);
            float GetVelocity();
            glm::vec3 GetAngularVelocity();
            void Update(float, float, float);
            float player_speed;
            bool forwards, left, right, backwards;
            bool collided;
            int lives;
            bool destroyed;
            bool attack;
            glm::vec3 direction;
     private:
        

     protected:
         Shader* shader;
         float velocity;
         glm::vec3 angular_velocity;
         glm::vec3 rotation;
         Texture2D* texture;
         vector<VertexFormat> vertices;
         vector<unsigned int> indices;
         glm::mat4 modelMatrix;
        // Mesh* head, body, right_hand, left_hand, right_leg, left_leg;
         float offset_body_x, offset_body_y, offset_body_z;
         float offset_hand_right_x, offset_hand_right_y, offset_hand_right_z;
         float offset_hand_left_x, offset_hand_left_y, offset_hand_left_z;
         float offset_leg_right_x, offset_leg_right_y, offset_leg_right_z;
         float offset_leg_left_x, offset_leg_left_y, offset_leg_left_z;

    };

