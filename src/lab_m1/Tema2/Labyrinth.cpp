#include "lab_m1/Tema2/Labyrinth.h"
#include "lab_m1/Tema2/Player.h"
#include "components/simple_scene.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Labyrinth::Labyrinth(float x, float y, float z, float s_x, float s_y, float s_z, WindowObject *window) {
	
    nr_obstacles = 0;
    NR_ENEMIES = 15;
	plane_pos_x = x;
	plane_pos_y = y;
	plane_pos_z = z;
	scale_plane_x = s_x;
	scale_plane_y = s_y;
	scale_plane_z = s_z;
	// Create a mesh box using custom data
	{
        glm::vec3 hot_pink = glm::vec3(1, 0.0783f, 0.576f);
        glm::vec3 light_pink = glm::vec3(1, 0.4117f, 0.7058f);
        vertices = vector<VertexFormat>
        {
            VertexFormat(glm::vec3(0, 0, 0), light_pink),
            VertexFormat(glm::vec3(1, 0, 0), light_pink),
            VertexFormat(glm::vec3(1, 1, 0),  hot_pink),
            VertexFormat(glm::vec3(0, 1, 0), light_pink),
            VertexFormat(glm::vec3(0, 0, 1),light_pink),
            VertexFormat(glm::vec3(1, 0, 1), hot_pink),
            VertexFormat(glm::vec3(1, 1, 1),light_pink),
            VertexFormat(glm::vec3(0, 1, 1),  hot_pink),
        };
        /*
		vertices = vector<VertexFormat>
		{
			VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1.f,1.f, 0.f)),
			VertexFormat(glm::vec3(1, 0, 0), glm::vec3(1.f,1.f, 0.f)),
			VertexFormat(glm::vec3(1, 1, 0),  glm::vec3(0.3f, 0.2f, 0.f)),
			VertexFormat(glm::vec3(0, 1, 0), glm::vec3(1.f,1.f, 0.f)),
			VertexFormat(glm::vec3(0, 0, 1),glm::vec3(1.f,1.f, 0.f)),
			VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0.3f, 0.2f, 0.f)),
			VertexFormat(glm::vec3(1, 1, 1),glm::vec3(1.f,1.f, 0.f)),
			VertexFormat(glm::vec3(0, 1, 1),  glm::vec3(0.3f, 0.2f, 0.f)),
		};
        */
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
       
		//since I created the initial plane cube of size one, I can calculate it's dimension after scaling
		SetPlaneDimenion(scale_plane_x, scale_plane_z);
        
	}
   
    
}

Labyrinth::Labyrinth() {


	
}
 vector<unsigned int> Labyrinth::getIndices() {
	return indices;
}
 vector<VertexFormat> Labyrinth::getVertices() {
	return vertices;
}
vector<Obstacle> Labyrinth::getObstaclesFromGrid(WindowObject* window) {
   // obstacles = vector<Obstacle>(nr_obstacles);
    PrintGrid();
    for (int i = 0; i < GRID_WIDTH; i++)
    {
        for (int j = 0; j < GRID_HEIGHT; j++)
        {
            if (grid[i * GRID_WIDTH + j] == '1') {
				pair<float, float> pos = GetPlaneCoords(i, j);
                printf(" OBJECT %f %f\n", pos.first, pos.second);
                Obstacle o(pos.first, plane_pos_y, pos.second, 1.f, 1.f, window);
                obstacles.push_back(o);
				nr_obstacles++;
            }
            else {
                printf("no\n");
            }
		
        }

	//	pair<float, float> pos = GetPlaneCoords(i, j);
		//SetInitialPlayerPos(pos.first, plane_pos_y, pos.second);
    }
    
	return obstacles;
}


Labyrinth::~Labyrinth()
{
}
pair<float, float> Labyrinth::GetPlaneDimension() {
	return pair<float, float>(plane_width, plane_height);
}
void Labyrinth::SetPlaneDimenion(float width, float height) {
	plane_width = width; plane_height = height;
}
void Labyrinth::SetInitialPlayerPos(float x, float z) {
	init_player_pos_x = x;
//	init_player_pos_y = y;
	init_player_pos_z = z;
	//printf("init lab x %f z %f\n", x, z);
}
tuple<float, float> Labyrinth::GetInitialPlayerPos() {
	return tuple<float, float>(init_player_pos_x, init_player_pos_z);
}
pair<float, float> Labyrinth::GetPlaneCoords(int x, int z) {
	//get the 3d plane position of an element in my grid matrix
	//ignore y because it remains the same as the plane's y
  //  printf("plane width %ff, plade height %f\n", plane_width, plane_height);
	float new_x = (float)x * (plane_pos_x + plane_width) / GRID_WIDTH;
	float new_z = (float)z * (plane_pos_z + plane_height) / GRID_HEIGHT;
	return pair<float, float>(new_x, new_z);
    //return pair<float, float>((float)x * plane_width / GRID_WIDTH, (float)y * plane_height / GRID_HEIGHT);
}

void Labyrinth::ResetGrid()
{
    // Fills the grid with walls ('#' characters).
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
    {
        grid[i] = '1';
    }
}

int Labyrinth::IsInBounds(int x, int y)
{
    // Returns "true" if x and y are both in-bounds.
    if (x < 0 || x >= GRID_WIDTH) return false;
    if (y < 0 || y >= GRID_HEIGHT) return false;
    return true;
}
int Labyrinth::XYToIndex(int x, int y) {
    return x * GRID_WIDTH + y;
}
pair<int, int> Labyrinth::generateRandomEntry() {
    int x = 1, y = 1;
    //first, select a border for the entry to be placed on
    //0 - left border
    //1 - right border
    //2 - upper border
    //3 - lower border
    int border = rand() % 4;
    
    printf("BORDER %d\n", border);
    //if the border is left/right, randomly select a x in the (1, GRID_WIDTH)
    //range and y is either 1 or GRID_HEIGHT
    switch (border) {
        case 0:
            x = 1;
            y = rand() % GRID_HEIGHT + 1;
            break;
        case 1:
            x = GRID_WIDTH;
            y = rand() % GRID_HEIGHT + 1;
            break;
        case 2:
            y = 1;
            x = rand() % GRID_WIDTH + 1;
            break;
        case 3:
            y = GRID_HEIGHT;
            x = rand() % GRID_WIDTH + 1;
            break;
    }
    printf("x = %d, y = %d\n", x, y);
    if (grid[x * GRID_WIDTH + y] != '1') {
        printf("ENTRY @(%d, %d)\n", x, y);
        return pair<int, int>(x, y);
    }
    else generateRandomEntry();
}
pair<int, int> Labyrinth::getRandomEntry() {
    return p;
}
// This is the recursive function we will code in the next project
void Labyrinth::Visit(int x, int y, float entrance)
{
 
    empty_cells.push_back(GetPlaneCoords(x, y));
    // Starting at the given index, recursively visits every direction in a
    // randomized order.
    // Set my current location to be an empty passage.
    grid[XYToIndex(x, y)] = '0';
   
    // Create an local array containing the 4 directionsF and shuffle their order.
    int dirs[4];
    dirs[0] = NORTH;
    dirs[1] = EAST;
    dirs[2] = SOUTH;
    dirs[3] = WEST;
    for (int i = 0; i < 4; ++i)
    {
        int r = rand() & 3;
        int temp = dirs[r];
        dirs[r] = dirs[i];
        dirs[i] = temp;
    }
    // Loop through every direction and attempt to Visit that direction.
    for (int i = 0; i < 4; ++i)
    {
        // dx,dy are offsets from current location. Set them based
        // on the next direction I wish to try.
        int dx = 0, dy = 0;
        switch (dirs[i])
        {
        case NORTH: dy = -1; break;
        case SOUTH: dy = 1; break;
        case EAST: dx = 1; break;
        case WEST: dx = -1; break;
        }
        // Find the (x,y) coordinates of the grid cell 2 spots
        // away in the given direction.
        int x2 = x + (dx << 1);
        int y2 = y + (dy << 1);
        if (IsInBounds(x2, y2))
        {
            if (grid[XYToIndex(x2, y2)] == '1')
            {
                // (x2,y2) has not been visited yet... knock down the
                // wall between my current position and that position
                grid[XYToIndex(x2 - dx, y2 - dy)] = '0';
                
                // Recursively Visit (x2,y2)
                Visit(x2, y2, entrance);
            }

        }
    }
}
void Labyrinth::PrintGrid()
{
    
    // Displays the finished maze to the screen.
    for (int y = 1; y <= GRID_HEIGHT; ++y)
    {
        printf("%d) ", y);
        for (int x = 1; x <= GRID_WIDTH; ++x)
        {
            if (p.first == x && p.second == y) {
                printf("FOUND ENTRY @(%d, %d)", x, y);
                cout << 'E';
            }
                
            else
                cout << grid[XYToIndex(x, y)];
        }
        cout << endl;
    }
}

char* Labyrinth::GetGrid() {
    return grid;
}

void Labyrinth::SetShader(Shader* s) {
    shader = s;
}
Shader* Labyrinth::GetShader() {
    return shader;
}
vector<pair<float, float>> Labyrinth::getEmptyCells() {
    return empty_cells;
}
vector<pair<float, float>> Labyrinth::getEnemiesPos() {
    int count = 0;
    int index = 0;
    while (count < NR_ENEMIES) {
        index = rand() % empty_cells.size();
        enemies_pos.push_back(empty_cells[index]);
       // printf("PLACE ENEMY @")
        count++;
    }
    return enemies_pos;
}
