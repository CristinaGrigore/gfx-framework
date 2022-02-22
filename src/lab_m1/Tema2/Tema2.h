#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/hw_camera.h"
#include "Player.h"
#include "Labyrinth.h"
#include "CollisionObject.h"
#include "Enemy.h"
#include "Progectile.h"
#include "HUD.h"
#include "transform2D.h"
#include "object2D.h"
#include <ctime>

 

using namespace std;



namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };
        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, glm::mat4) ;
  
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderPart(Mesh *mesh, float x, float y, float z, float scale_x, float scale_y, float scale_z, float angle, glm::mat4);
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void resetMovement(Player* p);
        void DisplayHUD(float deltaTimeSeconds);
        void DisplayGame(float deltaTimeSeconds);
        void printText(float x, float y, char* text);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear);
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void DrawScene(glm::mat3 visMatrix);

    protected:
         implemented::myCamera* camera;
         implemented::myCamera* second_camera;
         glm::mat4 projectionMatrix;
         glm::mat4 otherProjectionMatrix;
         bool renderCameraTarget;

         // TODO(student): If you need any other class variables, define them here.
         float fov, left, right, top, bottom, zNear, zFar;
         bool ortho = false;
         bool firstPerson;


        Player *player;
       // Enemy* enemy;
        Labyrinth* labyrinth;
        float angle;
        glm::vec3 axis;

        glm::mat4 targetMatrix;
        float centreX, centreY, centreZ;
        float offsetX, offsetY, offsetZ;
        
        vector<Obstacle> obstacles;

        float cell_length, cell_width;
        vector<pair<float, float>> enemies_pos;
        pair<float, float> pos;
        float MOVE_RIGHT, MOVE_DOWN;
        float deltaX, deltaY;
        vector<bool> collisions;
        vector<bool> collisions_enemies;
        bool universal_collision;
        CollisionObject::Direction lastDirection;
        int collided_object;
        int collided_enemy;
        //a vector which stores the collision direction  for each obstacle
        vector< CollisionObject::Direction> collision_directions;
        Progectile* p;
        bool shoot_progectile;
        float aux_angle = 0.0f, aux_x = 0.f, aux_y = 0.f, aux_z = 0.f;
        float time;
        bool shoot_progectile_again;
        float X, Y, Z;
        float MAX_DISTANCE = 10;
        bool progectile_collision = false;
        char hud_text[100];
        HUD* hud;
        bool renderHUD = false;
        float camera_angle = 0.f, total_camera_angle = 0.f;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 visMatrix, modelMatrix_2D;
        float life_scale_x, time_scale_x;
        float total_time;
    
        float test_scale_x, test_scale_y, test_scale_z;
        glm::mat4 bodyMatrix;
    };
}   // namespace m1
