#include "lab_m1/Tema1/Tema1.h"
 
#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/lab3/object2D.h"

using namespace std;
using namespace m1;
//a collision tople which contains if it happened, the direction it happened on and the direction vector
typedef std::tuple<bool, Direction, glm::vec2> Collision;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}
//directions for collisions

//a collision tople which contains if it happened and the direction it happened on
typedef std::tuple<bool, Direction, glm::vec2> Collision;
void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    cameraX_left = 0, cameraY_bottom = 0;
    cameraX_right = resolution.x; cameraY_top = resolution.y;
    //camera->SetOrthographic(0, (float)resolution.x/2, 0, (float)resolution.y/2, 0.01f, 400);
    camera->SetOrthographic(cameraX_left, cameraX_right, cameraY_bottom, cameraY_top, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();

    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    deltaTranslateX = 0; deltaTranslateY = 0;
    squareSide = 30.0f;

    cx = 15.0f;
    cy = 15.0f;

    cx_projectile = cx/4;
    cy_projectile = cy/4;
    // Initialize tx and ty (the translation steps)
    translateX = resolution.x/2;
    translateY = resolution.y/2;
    old_translateX = translateX; old_translateY = translateY;
    //initialize nr. of progectiles
    ammo = 100;
    //initialize coordinate vector
    coords = vector<Progectile_translation_coords>(ammo);
    for (size_t i = 0; i < ammo; i++)
    {
        coords.push_back({ 0, 0 });
    }
    //initialize nr. ob obstacles
    obstacle_count = 12;
    //initialize obstacles vector
    obstacles = vector<GameObject>(obstacle_count);
    for (size_t i = 0; i < obstacle_count; i++)
    {
        obstacles[i] = GameObject();
    }
    obstacles[0].pos = glm::vec2(200, 0);
    obstacles[0].size = glm::vec2(30*3, 30*4);
    obstacles[1].pos = glm::vec2(800, 40);
    obstacles[1].size = glm::vec2(2*30, 3*30);
    obstacles[2].pos = glm::vec2(600, 500);
    obstacles[2].size = glm::vec2(30 * 2, 30 * 2);
    obstacles[3].pos = glm::vec2(400, 150);
    obstacles[3].size = glm::vec2(30*3, 30*2);

    obstacles[4].pos = glm::vec2(930, 550);
    obstacles[4].size = glm::vec2(30 * 0.7f, 30 * 3);

    obstacles[5].pos = glm::vec2(80, 500);
    obstacles[5].size = glm::vec2(30 * 2, 30 * 4);

    obstacles[6].pos = glm::vec2(260, 200);
    obstacles[6].size = glm::vec2(30 * 3, 30 * 2);

    obstacles[7].pos = glm::vec2(850, 350);
    obstacles[7].size = glm::vec2(30 * 4, 30 * 3);

    //map left limit
    obstacles[8].pos = glm::vec2(-100, -10);
    obstacles[8].size = glm::vec2(100, 730);

    //map right limit
    obstacles[9].pos = glm::vec2(1290, -10);
    obstacles[9].size = glm::vec2(100, 730);

    //map up
    obstacles[10].pos = glm::vec2(-100, 730);
    obstacles[10].size = glm::vec2(1290, 100);

    //map down
    obstacles[11].pos = glm::vec2(0, -100);
    obstacles[11].size = glm::vec2(1290, 100);


    //initialize advance
    advance_right = true; advance_left = true;
    advance_up = true; advance_down = true;
    X = 0;
    Y = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;


    // Initialize angularStep
    angularStep = 0;

    //initialize shoot
    shoot = false;
    shoot_again = false;

    MAX_DISTANCE = 500;

    glm::vec3 corner_test = glm::vec3(0.001, 0.001, 0);
    length = 100.0f;
    time = 0;

    Mesh* test = object2D::CreateSquare("test", corner_test, length, glm::vec3(1, 0, 0), true);
    AddMeshToList(test);
   
    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1.0f, 0.5f, 0.31f), true);
    AddMeshToList(square1);

    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(1, 0.3f, 0.2f), true);
    AddMeshToList(square2);

    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0.9f, 0.3f, 0.3f), true);
    AddMeshToList(square3);

    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(0.9f, 0.3f, 0.3f), true);
    AddMeshToList(square4);

    Mesh* prog = object2D::CreateRectangle("prog", corner, squareSide/4, squareSide/4, glm::vec3(0.2f, 1, 0.3f), true);
    AddMeshToList(prog);

    Mesh* obstacle = object2D::CreateRectangle("obstacle", corner, 30, 30, glm::vec3(0.2f, 1, 0.3f), true);
    AddMeshToList(obstacle);

    Mesh* map = object2D::CreateRectangle("map", corner, resolution.x + 10, resolution.y + 10, glm::vec3(0.1f, 0.1f, 0), true);
    AddMeshToList(map);
    left_limit = 0, right_limit = resolution.x + 10, lower_limit = 0, upper_limit = resolution.y + 10;
    player = GameObject(glm::vec2(translateX, translateY), glm::vec2(squareSide, squareSide), glm::vec3(0.2f, 1, 0.3f), glm::vec2(0.2f, 1));
    progectile_object = GameObject(glm::vec2(translateX + cx + cx_projectile, translateY + cy + cy_projectile), glm::vec2(squareSide / 4, squareSide / 4));
  
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.1f, 0.3f, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    DrawScene(visMatrix, deltaTimeSeconds);

   // printf("%f %f\n", resolution.x, resolution.y);
}
void Tema1::Progectile(glm::mat3 modelMatrix, float deltaTimeSeconds, Progectile_translation_coords progectile) {
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(aux_X + cx + cx_projectile, aux_Y + cy + cy_projectile);
    glm::ivec2 resolution = window->GetResolution();
 

    if (getDistance(X, Y) >= MAX_DISTANCE || progectile_object.destroyed == true) {
        printf("progectile %d destroyed\n", ammo);
        ammo--;
   
        shoot = false;
        X = 0; Y = 0;
        return;
    }
    else shoot_again = false;
    if (progectile_object.destroyed != true) {
        OnInputUpdate(deltaTimeSeconds, 0);
        modelMatrix *= transform2D::Translate(X, Y);
        printf("%f %f\n", progectile_object.pos.x, progectile_object.pos.y);
        RenderMesh2D(meshes["prog"], shaders["VertexColor"], modelMatrix);
    }
    
}
float Tema1::getDistance(float x, float y) {
    return sqrt(x * x + y * y);
}
void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds) {
    auto camera = GetSceneCamera();
    camera->SetOrthographic(cameraX_left, cameraX_right, cameraY_bottom, cameraY_top, 0.01f, 400);
   
 
    modelMatrix *= transform2D::Translate(translateX + cx, translateY + cy);
    modelMatrix *= transform2D::Rotate(angularStep);
    modelMatrix *= transform2D::Translate(-cx, -cy);
    //translatia in centrul patratului (cx, cy) in origine

    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    //update player's position in the GameObject
    player.pos = glm::vec2(translateX  , translateY);
  
    player.size = glm::vec2(squareSide + 5, squareSide + 5);

    modelMatrix *= transform2D::Translate(+7, 0);

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2D::Scale(0.3f, 0.3f);
    modelMatrix *= transform2D::Translate(squareSide*2+30, 0);

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2D::Translate(0, squareSide*2+10);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    //obstacles
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(200, 0);
    modelMatrix *= transform2D::Scale(3, 4);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);;
    modelMatrix *= transform2D::Translate(800, 40);
    modelMatrix *= transform2D::Scale(2, 3);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(600, 500);
    modelMatrix *= transform2D::Scale(2, 2);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);;
    modelMatrix *= transform2D::Translate(400, 150);
    modelMatrix *= transform2D::Scale(3, 2);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);;
    modelMatrix *= transform2D::Translate(930, 550);
    modelMatrix *= transform2D::Scale(0.7f, 3);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);;
    modelMatrix *= transform2D::Translate(80, 500);
    modelMatrix *= transform2D::Scale(2, 4);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);;
    modelMatrix *= transform2D::Translate(260, 200);
    modelMatrix *= transform2D::Scale(3, 2);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);;
    modelMatrix *= transform2D::Translate(850, 350);
    modelMatrix *= transform2D::Scale(4, 3);
    RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], modelMatrix);
 
    //if mouse has been clicked and more than 0.5s passed since last click
    //and there's ammo left

    double oldDelta;

    time += deltaTimeSeconds;
    if (time >= 0.5f) {
        shoot_again = true;
        time = 0;
    }
    if (shoot == true && ammo - 1 >= 0) {
        Progectile(modelMatrix, deltaTimeSeconds, coords[ammo]); 
    } 
    DoCollisions();
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
}
void Tema1::FrameEnd()
{
}
void Tema1::printAdvance() {
    printf("left %d\nright%d\nup %d\ndown %d\n", advance_left, advance_right, advance_up, advance_down);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    auto camera = GetSceneCamera();
    float cameraSpeed = deltaTime * 100;
    if (window->KeyHold(GLFW_KEY_W) && advance_up) {
        cameraPos += cameraSpeed * cameraFront;
           translateY += deltaTime * 400.0f;
           cameraY_bottom += deltaTime * 400;
           camera->MoveUp(4000 * deltaTime);
          // printf("new camera Y bottom %f\ndelta*4000 = %f\ntransY = %f\n", cameraY_bottom, deltaTime * 4000, translateY);
           cameraY_top += deltaTime * 400;
    }else if (window->KeyHold(GLFW_KEY_S) && advance_down) {
            translateY -= deltaTime * 400;
            cameraY_bottom -= deltaTime * 400;
            cameraY_top -= deltaTime * 400;
     

    } else if (window->KeyHold(GLFW_KEY_A) && advance_left) {
            translateX -= deltaTime * 400;
            cameraX_left -= deltaTime * 400;
            cameraX_right -= deltaTime * 400;
           
          
    }
    else if (window->KeyHold(GLFW_KEY_D) && advance_right) {
            translateX += deltaTime * 400;
            cameraX_left += deltaTime * 400;
            cameraX_right += deltaTime * 400;
          
    }
    //"default" case is projectile shooting
    if (shoot == true) {
        float angle = atan2(coords[ammo].translateY_prog, coords[ammo].translateX_prog);  
        X += (coords[ammo].translateX_prog / 3 * deltaTime);
        Y += coords[ammo].translateY_prog / 3 * deltaTime;
        progectile_object.pos.x += (coords[ammo].translateX_prog / 3 * deltaTime);
        progectile_object.pos.y += (coords[ammo].translateX_prog / 3 * deltaTime);
    }
   
}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 resolution = window->GetResolution();
    //get angle between mouse position and and obj position
    float dx = translateX + cx, dy = translateY + cy;
    
    float mouse_x = (float)mouseX, mouse_y = resolution.y - (float)mouseY;
  //  printf("X %f, Y %f\n", (float)mouseX + cameraX_left, resolution.y - (float)mouseY + cameraY_bottom);
  //  printf("cameraX_left %f, cameraY_bottom %f\n", cameraX_left, cameraY_bottom);
    angularStep = -atan2((float)(mouseY - cameraY_bottom - dy), (float)(mouseX + cameraX_left - dx));

}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    glm::ivec2 resolution = window->GetResolution();
    float dx = translateX + cx , dy = translateY + cy;
    
    if (button == GLFW_MOUSE_BUTTON_2 && shoot == false) {
        printf("players pos %f %f\n", translateX + cameraX_left, translateY);

       // printf("shoot %f deltaCamX = %f\n", (float)( mouse_x ), deltaCameraX);
        deltaTranslateX += (old_translateX - translateX);
        deltaTranslateY += (old_translateY - translateY);
       // printf("old %f, new %f\n", old_translateX, translateX);
        mouse_y = resolution.y - (float)mouseY;
        mouse_x = (float)mouseX;
       // printf("X %f, Y %f\ndelta TransX =  %f, delta TransY %f\n", mouse_x,deltaTranslateX, mouse_y, deltaTranslateY);
        mouse_x = mouse_x + cameraX_left;
        mouse_y = mouse_y + cameraY_bottom;
  
        coords[ammo].translateX_prog =  mouse_x - dx;
        coords[ammo].translateY_prog =  mouse_y - dy;
      //  printf("transX = %f transY = %f\n%f %f\n", translateX_prog, translateY_prog, (float)mouseX, (float)(resolution.y - mouseY));
        shoot = true;
        
        aux_X = translateX; aux_Y = translateY;
        aux_angular = angularStep;
        
        progectile_object.pos = glm::vec2(aux_X + cx + cx_projectile, aux_Y + cy + cy_projectile);
        if (progectile_object.destroyed) {
            printf("IS DESTROYED\n");
            progectile_object.destroyed = false;
            progectile_object.pos = glm::vec2(aux_X + cx + cx_projectile, aux_Y + cy + cy_projectile);
       }
    }

}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
//2D vizualization matrix
// Uniform 2D visualization matrix (same scale factor on x and y axes)


Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }

    return (Direction)best_match;
}

Collision Tema1::CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // get center point circle first 
    glm::vec2 center(one.pos + one.size * 0.5f);
    // calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
    glm::vec2 aabb_center(two.pos.x + aabb_half_extents.x, two.pos.y + aabb_half_extents.y);
    // get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (glm::length(difference) < cx) {
        // not <= since in that case a collision also occurs when object one exactly touches object two,
        //which they are at the end of each collision resolution stage.

        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Tema1::DoCollisions() {
    bool collide = false;
    if (!player.destroyed) {
       
        for (int i = 0; i < obstacle_count; i++) {
            Collision collision = CheckCollision(player, obstacles[i]);
            Collision collision2 = CheckCollision(progectile_object, obstacles[i]);
          
            if (std::get<0>(collision)) {
                collide = true;
                printf("COLLIDE obj %d\n", i);
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                switch (dir) {
                case LEFT:
                    advance_left = false;
                    break;
                case RIGHT:
                    advance_right = false;
                    break;
                case UP:
                    advance_up = false;
                    break;
                case DOWN:
                    advance_down = false;
                    break;
               }
                printf("DIR "); cout << dir << endl;
                printAdvance();
             
            }
            if (std::get<0>(collision2) && progectile_object.destroyed == false) {

                //if a progectile collides with one of the obstacles
                progectile_object.destroyed = true;
                
                
            }
          
        }
        if (collide == false) {
            advance_down = true; advance_left = true; advance_right = true; advance_up = true;
        }
        
    }
}