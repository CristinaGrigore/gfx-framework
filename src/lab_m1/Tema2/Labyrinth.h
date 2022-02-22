#pragma once
#include <iostream>
#include <vector>
#include "Obstacle.h"
#include "components/simple_scene.h"
#include "core/gpu/mesh.h"

//ONLY WORKS FOR ODD VALUES OF WIDTH AND HEIGHT
#define GRID_WIDTH 17
#define GRID_HEIGHT 23
#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
using namespace std;
 class Labyrinth
    {
     public:
         int nr_obstacles;
         int NR_ENEMIES;
         vector<Obstacle> obstacles;
         float plane_pos_x, plane_pos_y, plane_pos_z, scale_plane_x, scale_plane_y, scale_plane_z;
         //only render plane
         Labyrinth();
         Labyrinth(float x, float y, float z, float s_x, float s_y, float s_z, WindowObject* window);
         ~Labyrinth();
          vector<VertexFormat> getVertices();
          vector<unsigned int> getIndices();
          void SetPlaneDimenion(float, float);
          pair<float, float> GetPlaneDimension();
          pair<float, float> GetPlaneCoords(int x, int y);
          vector<Obstacle> getObstaclesFromGrid(WindowObject* window);
          void SetInitialPlayerPos(float x, float z);
          tuple<float, float> GetInitialPlayerPos();
          void ResetGrid();
          int IsInBounds(int x, int y);
          int XYToIndex(int x, int y);
          void Visit(int x, int y, float entrance);
          void PrintGrid();
          char* GetGrid();
          pair<int, int> generateRandomEntry();
          pair<int, int> getRandomEntry();
          void SetShader(Shader* s);
          Shader* GetShader();
          vector<pair<float, float>> getEmptyCells();
          vector<pair<float, float>> getEnemiesPos();
          //random entry
          pair<int, int> p;

     private:
        

     protected:
         float plane_width, plane_height;
         float init_player_pos_x, init_player_pos_y, init_player_pos_z;
          vector<VertexFormat> vertices;
          vector<unsigned int> indices;
          
          char grid[GRID_WIDTH * GRID_HEIGHT];
          Shader* shader;
          vector<pair<float, float>> empty_cells;
          vector<pair<float, float>> enemies_pos;
          
    };

