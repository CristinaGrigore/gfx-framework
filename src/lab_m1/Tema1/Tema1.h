#pragma once

#include "components/simple_scene.h"
#include "GameObject.h"

#include <vector>
#include <iostream>
#include <chrono>
//directions for collisions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
typedef std::tuple<bool, Direction, glm::vec2> Collision;
namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
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


        struct Progectile_translation_coords
        {
            float translateX_prog;
            float translateY_prog;
        };
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void DrawScene(glm::mat3, float);
        glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
        void Tema1::Progectile(glm::mat3 modelMatrix, float deltaTimeSeconds, Progectile_translation_coords progectile);
        float getDistance(float x, float y);
        Collision CheckCollision(GameObject& one, GameObject& two);
        void DoCollisions();
        void printAdvance();
    protected:
        float cx, cy, cx_projectile, cy_projectile;
        int dir, orientation;
        glm::mat3 modelMatrix, rotMatrix, auxMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        float PI = 3.14159;
        float upper_limit, lower_limit, left_limit, right_limit;
        int xpos, ypos;
        float X, Y;
        float projectile_start_time;
        float cameraX_left, cameraY_bottom, cameraX_right, cameraY_top;
        bool shoot, shoot_again;
        float mouse_x, mouse_y;
        float deltaTranslateX, old_translateX, old_translateY, deltaTranslateY;

        float length, time;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3  visMatrix;
        float MAX_DISTANCE;
        //number of bullets
        int ammo;
        float squareSide;
        //advance = true if there's no collision between player and obstacles
        bool advance_left, advance_right, advance_up, advance_down;
        float aux_Y, aux_X, aux_angular;
        //float lower_limit, upper_limit, left_limit, right_limit;

        GameObject player, progectile_object;
        std::vector<GameObject> obstacles;
        int obstacle_count;
        //array of translation coords for the progectiles
        std::vector<Progectile_translation_coords> coords;
        glm::vec2 difference;
  
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;
        glm::mat4 view;
    };
}   // namespace m1
