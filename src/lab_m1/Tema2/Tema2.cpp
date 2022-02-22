#include "lab_m1/Tema2/Tema2.h"
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    labyrinth = new Labyrinth(0.f, -0.19f, 0.f, 60.f, 0.2f, 60.f, window);
    labyrinth->ResetGrid();
    labyrinth->Visit(1, 1, false);
    labyrinth->p = labyrinth->generateRandomEntry();
    labyrinth->PrintGrid();
    pair<int, int> entry = labyrinth->p;
    pair<float, float> entry_in_my_coords = labyrinth->GetPlaneCoords(entry.first, entry.second);
    player = new Player(entry_in_my_coords.first + 0.5f, 1.2f, entry_in_my_coords.second + 0.5f, window);

    X = 0.15f;
    Y = -1.9f;
    Z = 0.f;
    //Progectile
    p = new Progectile(player->pos_body_x + X, player->pos_body_y + Y, player->pos_body_z + Z, 0.2f, window);
    meshes[p->getMesh()->GetMeshID()] = p->getMesh();

    //Shaders
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    ortho = false;
    //init rotatio
    right = 10.f;
    left = .01f;
    bottom = .01f;
    top = 10.f;
    fov = 40.f;
    angle = 0.0f;
    zNear = 0.01f; zFar = 200.0f;
    //init axis for rotation
    axis = glm::vec3(0, 1, 0);

    targetMatrix = glm::mat4(1);
    offsetX = offsetY = offsetZ = 0;
    centreX = -4.5f;
    centreZ = 0.f;
    centreY = 1.75f;

    firstPerson = false;

    shoot_progectile = false;
    shoot_progectile_again = false;
    //init all obstacles
    obstacles = labyrinth->getObstaclesFromGrid(window);

    //init enemies
    enemies_pos = labyrinth->getEnemiesPos();

    camera = new implemented::myCamera();
    second_camera = new implemented::myCamera();
    glm::vec3 position = glm::vec3(player->pos_head_x, player->pos_head_y + 0.9f, player->pos_head_z + 3.2f);
    glm::vec3 center = glm::vec3(player->pos_head_x, player->pos_head_y, player->pos_head_z);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
    camera->Set(position, center, up);
    std::printf("init x %f y %f z %f\n", player->pos_head_x, player->pos_head_y, player->pos_head_z);

    // camera->Set(glm::vec3(player->pos_body_x - 1.3f, 2.f, player->pos_body_z - 2.f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
     // TODO(student): After you implement the changing of the projection
     // parameters, remove hardcodings of these parameters
    // projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);

    pair<float, float> p1 = labyrinth->GetPlaneCoords(1, 1);
    pair<float, float> p2 = labyrinth->GetPlaneCoords(1, 2);

    cell_length = p2.second - p1.second;

    p1 = labyrinth->GetPlaneCoords(0, 0);
    p2 = labyrinth->GetPlaneCoords(1, 0);
    cell_width = p2.first - p1.first;

    //The 4 corners on the plane should correspond to the 4 corners of the grid
    p1 = labyrinth->GetPlaneCoords(0, 23);
    p1 = labyrinth->GetPlaneCoords(17, 23);
    p1 = labyrinth->GetPlaneCoords(17, 0);

    MOVE_RIGHT = true;
    MOVE_DOWN = true;
    deltaX = 0.f; deltaY = cell_length - player->scale_body_z;

    universal_collision = false;
    collisions = vector<bool>(labyrinth->nr_obstacles, false);
    collisions_enemies = vector<bool>(labyrinth->NR_ENEMIES + 1, false);
    collided_object = -1;
    collided_enemy = -1;
    collision_directions = vector<CollisionObject::Direction>(max(labyrinth->nr_obstacles, labyrinth->NR_ENEMIES));

    time = 0.f;
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab6", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab6", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;
    Mesh* square1 = object2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);
    Mesh* square2 = object2D::CreateSquare("square2", corner, squareSide, glm::vec3(1, 0, 0), false);
    AddMeshToList(square2);
    Mesh* square3 = object2D::CreateSquare("square3", corner, squareSide, glm::vec3(0, 0.7960f, 1), true);
    AddMeshToList(square3);
    Mesh* square4 = object2D::CreateSquare("square4", corner, squareSide, glm::vec3(1, 0.7960f, 1), false);
    AddMeshToList(square4);
    life_scale_x = 0.03f;
    time_scale_x = 0.03f;
    total_time = 0.f;
    {
        Mesh* mesh = new Mesh("heart");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Love.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("IronMan");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "IronMan.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    test_scale_x = 0.5f; test_scale_y = 0.3f; test_scale_z = 0.4f;
    total_camera_angle = 0.f;
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer

    glClearColor(0.6784f, 0.847f, 0.901f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema2::RenderPart(Mesh* mesh, float x, float y, float z, float scale_x, float scale_y, float scale_z, float angle, glm::mat4 projectionMatrix) {
    glm::mat4 modelMatrix = glm::mat4(1);
    
    if (!strcmp(mesh->GetMeshID(), "progectile") ) {
         if (shoot_progectile) {
            angle = aux_angle;
            x = aux_x;
            y = aux_y;
            z = aux_z;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(x + scale_x / 2,y + scale_y / 2, z + scale_z / 2));
            modelMatrix = glm::rotate(modelMatrix, (float)(angle), axis);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(X - scale_x / 2, Y - scale_y / 2, Z - scale_z / 2));
        }
        else {
            angle = total_camera_angle;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(player->pos_body_x + scale_x / 2, player->pos_body_y + scale_y / 2, player->pos_body_z + scale_z / 2));
            modelMatrix = glm::rotate(modelMatrix, (float)(angle), axis);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(X - scale_x / 2, Y - scale_y / 2, Z - scale_z / 2));
        }

    }
    /*
    if(!strcmp(mesh->GetMeshID(), "progectile")) {
        modelMatrix = bodyMatrix;
        if (shoot_progectile) {
            angle = aux_angle;
            printf("aux %f\n", aux_angle);
        }
        else {
            printf("total %f\n", total_camera_angle);
            angle = total_camera_angle;
        }
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-player->scale_body_x/2 - scale_x/2 + X, Y, -player->scale_body_z/2 -scale_z / 2 + Z));
       
    } 
    */
    else {
    
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x + scale_x / 2, y + scale_y / 2, z + scale_z / 2));

        modelMatrix = glm::rotate(modelMatrix, (float)(angle), axis);

        modelMatrix = glm::translate(modelMatrix, glm::vec3(-scale_x / 2, -scale_y / 2, -scale_z / 2));
        if (!strcmp(mesh->GetMeshID(), "player_body")) {
            bodyMatrix = modelMatrix;
        }
    }
      modelMatrix = glm::scale(modelMatrix, glm::vec3(scale_x, scale_y, scale_z));


    if (!strcmp(mesh->GetMeshID(), "plane")) {

        RenderMesh(mesh, shaders["VertexColor"], modelMatrix, projectionMatrix);

    }
    else if (strstr(mesh->GetMeshID(), "player") != NULL) {

        // modelMatrix = glm::rotate(modelMatrix, (float)(-camera_angle * 180 / M_PI), axis);
        RenderMesh(mesh, shaders["VertexColor"], modelMatrix, projectionMatrix);

    }
    else if (!strcmp(mesh->GetMeshID(), "obstacle")) {

        RenderMesh(mesh, shaders["VertexColor"], modelMatrix, projectionMatrix);
    }
    else if (!strcmp(mesh->GetMeshID(), "progectile")) {

        RenderMesh(mesh, shaders["LabShader"], modelMatrix, projectionMatrix);
    }
    else if (!strcmp(mesh->GetMeshID(), "square1")) {

        RenderMesh2D(mesh, shaders["VertexColor"], modelMatrix);
    }
    else {
        RenderMesh(mesh, shaders["VertexNormal"], modelMatrix, projectionMatrix);

    }

}
void printDir(CollisionObject::Direction d) {
    switch (d) {
    case 0: std::printf("UP\n");
        break;
    case 1: std::printf("RIGHT\n");
        break;
    case 2:
        std::printf("DOWN\n");
        break;
    case 3:
        std::printf("LEFT\n");
        break;
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    total_time += deltaTimeSeconds;
    time_scale_x -= deltaTimeSeconds * 0.03 / 180;
    if (time_scale_x <= 0) {
        std::printf("GAME OVER\n");
        exit(-1);
    }
    DisplayGame(deltaTimeSeconds);
    DisplayHUD(deltaTimeSeconds);
    //glutSwapBuffers();
}
void Tema2::DisplayGame(float deltaTimeSeconds) {

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);
    {
        //test mesh for progectile shooting
        CreateMesh("test", player->getVertices(), player->getIndices());

        CreateMesh("player_body", player->getVertices(), player->getIndices());
        CreateMesh("player_hands", player->getVertices(), player->getIndices());
        CreateMesh("player_legs", player->getVertices(), player->getIndices());
        CreateMesh("player", player->getVertices(), player->getIndices());
        ((CollisionObject*)player)->SetBoundingBox(player->pos_body_x - 0.2f, player->pos_body_x + player->scale_body_x + 0.2f, player->pos_leg_left_y, player->pos_head_y, player->pos_body_z - 0.2f, player->pos_body_z + player->scale_body_z + 0.2f);

        /*Render player*/

        if (firstPerson == false) {
            glm::mat4 modelMatrix = glm::mat4(1);
            camera->TranslateForward(3.f);
            player->Update(camera->position.x, camera->position.y, camera->position.z);
            //constanly update progectile position
            p->pos_x = player->pos_body_x;
            p->pos_y = player->pos_body_y;
            p->pos_z = player->pos_body_z;
            camera->TranslateForward(-3.f);

        }
        else {
            //first person
            player->Update(camera->position.x, camera->position.y, camera->position.z);
            p->pos_x = player->pos_body_x;
            p->pos_y = player->pos_body_y;
            p->pos_z = player->pos_body_z;
        }
        
        RenderPart(meshes["player"], player->pos_head_x, player->pos_head_y, player->pos_head_z, player->scale_head, player->scale_head, player->scale_head, total_camera_angle, projectionMatrix);
        RenderPart(meshes["player_body"], player->pos_body_x, player->pos_body_y, player->pos_body_z, player->scale_body_x, player->scale_body_y, player->scale_body_z, total_camera_angle, projectionMatrix);
        //RenderPart(meshes["player_hands"], player->pos_hand_right_x, player->pos_hand_right_y, player->pos_hand_right_z, player->scale_body_x / 3, player->scale_body_y, player->scale_body_z / 3, total_camera_angle, projectionMatrix);
       // RenderPart(meshes["player_hands"], player->pos_hand_left_x, player->pos_hand_left_y, player->pos_hand_left_z, player->scale_body_x / 3, player->scale_body_y, player->scale_body_z / 3, total_camera_angle, projectionMatrix);
        RenderPart(meshes["player_legs"], player->pos_leg_right_x, player->pos_leg_right_y, player->pos_leg_right_z, player->scale_leg_x, player->scale_leg_y, player->scale_leg_z, total_camera_angle, projectionMatrix);
        RenderPart(meshes["player_legs"], player->pos_leg_left_x, player->pos_leg_left_y, player->pos_leg_left_z, player->scale_leg_x, player->scale_leg_y, player->scale_leg_z, total_camera_angle, projectionMatrix);
  
        RenderPart(meshes["heart"], player->pos_body_x, player->pos_head_y - 30, player->pos_head_z, 3, 2, 3, 0.f, projectionMatrix);
        angle += deltaTimeSeconds * 8.f;
        RenderPart(meshes["heart"], player->pos_head_x + 0.001f, player->pos_head_y + 0.3f, player->pos_head_z, 0.006f, 0.006f, 0.006f, angle, projectionMatrix);

        /*Render plane*/
        CreateMesh("plane", labyrinth->getVertices(), labyrinth->getIndices());
        RenderPart(meshes["plane"], labyrinth->plane_pos_x, labyrinth->plane_pos_y, labyrinth->plane_pos_z, labyrinth->scale_plane_x, labyrinth->scale_plane_y, labyrinth->scale_plane_z, 0.f, projectionMatrix);

        /*Render Progectile*/
        float x = player->pos_body_x - player->scale_body_x / 2 - p->scale / 2 + X - 0.2f;
        float X_max = player->pos_body_x - player->scale_body_x / 2 + p->scale / 2 + X + 0.2f;
        float y = player->pos_body_y + Y - 0.2f;
        float Y_max = player->pos_body_y + Y + p->scale + 0.2f;
        float z = player->pos_body_z - player->scale_body_z / 2 - p->scale / 2 + Z - 0.2f;
        float Z_max = player->pos_body_z - player->scale_body_z / 2 + p->scale / 2 + Z + 0.2f;
        ((CollisionObject*)p)->SetBoundingBox(x, X_max, y, Y_max, z, Z_max);

        if(firstPerson && shoot_progectile)
            RenderPart(meshes["progectile"], (p->pos_x + X), p->pos_y + Y, (p->pos_z + Z), p->scale, p->scale, p->scale, total_camera_angle, projectionMatrix);

        CreateMesh("obstacle", obstacles[0].getVertices(), obstacles[0].getIndices());
        for (int i = 0; i < obstacles.size(); i++)
        {
            Obstacle o = obstacles[i];
            ((CollisionObject*)&o)->SetBoundingBox(o.pos_x - 0.2f, o.pos_x + cell_width + 0.2f, o.pos_y, o.pos_y + 1.2f + 0.2f, o.pos_z - 0.2f, o.pos_z + cell_length + 0.2f);
            bool collision = ((CollisionObject*)player)->CheckCollision((CollisionObject*)&o);
            if (collision) {
                collided_object = i;
            }

            collisions[i] = collision;

            if (collided_object >= 0 && collisions[collided_object]) {
                // printf("COLIDDED ON ");
               // printDir(lastDirection);
                collision_directions[collided_object] = lastDirection;
                player->OnCollision(lastDirection);
            }
            if (player->collided && collided_object >= 0 && collisions[collided_object] == false) {
                std::printf("Resolve collision on dir ");
                //printDir(collision_directions[collided_object]);
                player->resolveCollision(collision_directions[collided_object]);
                player->collided = false;
                collided_object = -1;

            }
            RenderPart(meshes["obstacle"], o.pos_x, o.pos_y, o.pos_z, cell_width, 1.2f, cell_length, 0.f, projectionMatrix);

        }

        for (int i = 0; i < labyrinth->NR_ENEMIES; i++) {
            pos = enemies_pos[i];

            if (MOVE_RIGHT && MOVE_DOWN) {

                // pos.first += 2 * deltaTimeSeconds;
                deltaX += 0.04 * deltaTimeSeconds;
                //1 is the enemy's scale on Ox
                if (deltaX >= cell_width - 1) {
                    MOVE_RIGHT = false;
                    MOVE_DOWN = true;

                }
            }
            if (MOVE_DOWN && !MOVE_RIGHT) {

                deltaY -= 0.04 * deltaTimeSeconds;
                if (deltaY <= 0) {
                    MOVE_DOWN = false;
                    MOVE_RIGHT = false;
                }
            }
            if (!MOVE_RIGHT && !MOVE_DOWN) {


                deltaX -= 0.04 * deltaTimeSeconds;
                if (deltaX <= 0)
                    MOVE_RIGHT = true;
            }
            if (!MOVE_DOWN && MOVE_RIGHT) {

                deltaY += 0.04 * deltaTimeSeconds;
                //1 is the enemy's scale on Oz
                if (deltaY >= cell_length - 1) {
                    MOVE_RIGHT = true;
                    MOVE_DOWN = true;

                }

            }

            Enemy* enemy = new Enemy(pos.first + deltaX, labyrinth->plane_pos_y, pos.second + deltaY, 1, player->scale_body_y * 3, 1, window);
            ((CollisionObject*)enemy)->SetBoundingBox(enemy->pos_x - 0.2f, enemy->pos_x + enemy->scale_x + 0.2f, enemy->pos_y, enemy->pos_y + enemy->scale_y + 0.2f, enemy->pos_z - 0.2f, enemy->pos_z + enemy->scale_z + 0.2f);
            CreateMesh("enemy", enemy->getVertices(), enemy->getIndices());
            RenderPart(meshes["enemy"], enemy->pos_x, enemy->pos_y, enemy->pos_z, enemy->scale_x, enemy->scale_y, enemy->scale_z, 0.f, projectionMatrix);

            bool collision = ((CollisionObject*)player)->CheckCollision((CollisionObject*)enemy);
            bool local_progectile_collision = ((CollisionObject*)p)->CheckCollision((CollisionObject*)enemy);
            if (local_progectile_collision) {
                progectile_collision = true;
                printf("PROG COLLIDED W/ ENEMY\n");
            }
            if (collision) {
                collided_enemy = i;
                printf("PLAYER COLLIDED W/ ENEMY\n");
            }
            collisions_enemies[i] = collision;


            if (collided_enemy >= 0 && collisions_enemies[collided_enemy]) {

                // printDir(lastDirection);
                collision_directions[collided_enemy] = lastDirection;
                 player->lives--;
                life_scale_x -= 0.03 / 1000;
               // std::printf("lives %d\n ", player->lives);

                if (player->lives <= 0) {
                    player->destroyed = true;
                    std::printf("GAME OVER\n");
                    exit(-1);
                }
                else
                    player->OnCollision(lastDirection);
            }

            if (player->collided && collided_enemy >= 0) {
                if (collisions_enemies[collided_enemy] == false) {
                    std::printf("Resolve collision on dir ");
                    printDir(collision_directions[collided_enemy]);
                    player->resolveCollision(collision_directions[collided_enemy]);
                    player->collided = false;
                    collided_enemy = -1;
                }


            }


            // RenderPart(meshes["IronMan"], pos.first + deltaX, labyrinth->plane_pos_y, pos.second + deltaY, 0.01f, 0.01f, 0.01f, 0.f, projectionMatrix);
        }

    }


}

void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);

}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);


    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::mat4 projectionMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // printf("mesh id %s\n", mesh->GetMeshID());
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

float getDistance(float x, float y) {
    return sqrt(x * x + y * y);
}
void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    float cameraSpeed = player->GetVelocity();
    float progectile_speed = 4.f;
    if (getDistance(X, Z) > MAX_DISTANCE || progectile_collision) {
        Z = 0.f;
        X = 0.15f;
        shoot_progectile = false;
        shoot_progectile_again = true;
    }
    else if (shoot_progectile && firstPerson && getDistance(X, Z) <= MAX_DISTANCE) {
        Z -= progectile_speed * deltaTime;
        
        shoot_progectile_again = false;
    }
    if (progectile_collision && Z == 0.3f) {
       // printf("roj collision\n");
        progectile_collision = false;
    }


    if (window->KeyHold(GLFW_KEY_W) && player->forwards == true) {

        camera->MoveForward(cameraSpeed * deltaTime);


        if (!player->collided)
            lastDirection = CollisionObject::Direction::UP;
    }


    if (window->KeyHold(GLFW_KEY_A) && player->left == true) {

        camera->TranslateRight(-cameraSpeed * deltaTime);


        if (!player->collided)
            lastDirection = CollisionObject::Direction::LEFT;
        //  else if()
    }

    if (window->KeyHold(GLFW_KEY_S) && player->backwards == true) {


        camera->MoveForward(-cameraSpeed * deltaTime);

        if (!player->collided)
            lastDirection = CollisionObject::Direction::DOWN;


    }

    if (window->KeyHold(GLFW_KEY_D) && player->right == true) {


        camera->TranslateRight(cameraSpeed * deltaTime);

        if (!player->collided)
            lastDirection = CollisionObject::Direction::RIGHT;

    }


    if (window->KeyHold(GLFW_KEY_Q)) {
        // TODO(student): Translate the camera downward
        camera->TranslateUpward(cameraSpeed * deltaTime);

    }

    if (window->KeyHold(GLFW_KEY_E)) {
        // TODO(student): Translate the camera upward
        camera->TranslateUpward(-cameraSpeed * deltaTime);
    }
    if (window->KeyHold(GLFW_KEY_V)) {
        std::printf("PLAYER POS x: %f, PLAYER POS Z: %f\n", player->pos_leg_right_x, player->pos_leg_left_z);
    }

    if (window->KeyHold(GLFW_KEY_1)) {
        if (fov <= 180)
            fov += deltaTime;

    }
    if (window->KeyHold(GLFW_KEY_2)) {
        fov -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_3)) {
        left -= deltaTime;
        right += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_4)) {
        left += deltaTime;
        right -= deltaTime;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_LEFT)
        player->SetAngularVelocity(glm::vec3(0, 1, 0));
    if (key == GLFW_KEY_RIGHT)
        player->SetAngularVelocity(glm::vec3(0, -1, 0));
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    if (key == GLFW_KEY_K) {
        labyrinth->p = labyrinth->generateRandomEntry();
        pair<int, int> entry = labyrinth->p;
        labyrinth->PrintGrid();
        pair<float, float> entry_in_my_coords = labyrinth->GetPlaneCoords(entry.first, entry.second);
        player = new Player(entry_in_my_coords.first, 1.2f, entry_in_my_coords.second, window);

        glm::vec3 position = glm::vec3(player->pos_head_x, player->pos_head_y + 0.9f, player->pos_head_z + 3.2f);
        glm::vec3 center = glm::vec3(player->pos_head_x, player->pos_head_y, player->pos_head_z);
        glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
        camera->Set(position, center, up);
        total_camera_angle = 0.f;
    }

    if (key == GLFW_KEY_LEFT_CONTROL) {
        firstPerson = !firstPerson;
        if (firstPerson) {
            //  camera->RotateFirstPerson_OY(total_camera_angle);
            camera->MoveForward(camera->distanceToTarget);
            player->attack = true;
        }
        else
            camera->MoveForward(-camera->distanceToTarget);


    }

    if (key == GLFW_KEY_SPACE && firstPerson) {
        aux_angle = total_camera_angle;
        aux_x = p->pos_x;
        aux_y = p->pos_y;
        aux_z = p->pos_z;
        shoot_progectile = true;
    }
}

void Tema2::OnKeyRelease(int key, int mods)
{
    if (key == GLFW_KEY_LEFT)
        player->SetAngularVelocity(glm::vec3(0));

    if (key == GLFW_KEY_RIGHT)
        player->SetAngularVelocity(glm::vec3(0));
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;


    glm::ivec2 resolution = window->GetResolution();

    float offsetx = mouseX - resolution.x / 2;
    float offsety = resolution.y - mouseY;
    camera_angle = (float)deltaX * 0.01f;

    if (firstPerson) {
        total_camera_angle += camera_angle;

        if (total_camera_angle > 360.0f) {
            total_camera_angle -= 360.0f;
        }
        if (total_camera_angle < -360.0f) {
            total_camera_angle += 360.0f;
        }
        renderCameraTarget = false;
        camera->RotateFirstPerson_OY(camera_angle);

    }

    if (!firstPerson) {
        total_camera_angle += camera_angle;

        if (total_camera_angle > 360.0f) {
            total_camera_angle -= 360.0f;
        }
        if (total_camera_angle < -360.0f) {
            total_camera_angle += 360.0f;
        }
        camera->RotateThirdPerson_OY(camera_angle);

    }
    // printf("angle %d\n", angle);



}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
void Tema2::resetMovement(Player* player) {
    player->forwards = true;    player->backwards = true;
    player->left = true;        player->right = true;
}


void Tema2::DisplayHUD(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    glDisable(GL_DEPTH_TEST);
    // Setam alt viewport
// glViewport(0, 0, resolution.x, resolution.y);
    glViewport(-100, resolution.y - 90, 500, 300);
    ortho = true;
    {
        projectionMatrix = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, -1.f, 1.f);
        modelMatrix_2D = glm::mat3(1);
        modelMatrix_2D *= transform2D::Translate(-1.0f, 0.0f);
        modelMatrix_2D *= transform2D::Scale(life_scale_x, 0.002f);

        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix_2D);

        modelMatrix_2D = glm::mat3(1);
        modelMatrix_2D *= transform2D::Translate(-1.0f, 0.0f);
        modelMatrix_2D *= transform2D::Scale(0.03f, 0.002f);

        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix_2D);
        //projectionMatrix = glm::ortho(0.0f, resolution.x - 300, 0.0f, resolution.y - 300, -1.f, 1.f);
    }
    {
        //remaining time

        modelMatrix_2D = glm::mat3(1);
        modelMatrix_2D *= transform2D::Translate(-1.0f, -0.40f);
        modelMatrix_2D *= transform2D::Scale(time_scale_x, 0.002f);

        RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix_2D);

        modelMatrix_2D = glm::mat3(1);
        modelMatrix_2D *= transform2D::Translate(-1.0f, -0.40f);
        modelMatrix_2D *= transform2D::Scale(0.03f, 0.002f);

        RenderMesh2D(meshes["square4"], shaders["VertexColor"], modelMatrix_2D);
    }

    glEnable(GL_DEPTH_TEST);


}


