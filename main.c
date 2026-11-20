#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>

#define GRAVITY 20
#define JUMP_HEIGHT 2

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define X_SIZE 50
#define Y_SIZE 60
#define Z_SIZE 50

#define AIR -1
#define GRASS 0
#define DIRT 1
#define STONE 2

typedef struct {
    int id;
    char* name;
    Texture texture;
} Bloc;

typedef struct {
    Vector3 velocity;
    Vector3 position; 
} Player;

static int map[X_SIZE][Y_SIZE][Z_SIZE];

int getBlock(int x, int y, int z) {
    int block = -1;
    if(x < X_SIZE && x >= 0 && z < Z_SIZE && z >= 0 && y < Y_SIZE && y >= 0) block = map[x][y][z];
    return block;
}

bool isOnFloor(Player player) {
  if(getBlock((int) floorf(player.position.x), (int) floorf(player.position.y - 0.05f), (int) floorf(player.position.z)) == AIR) 
    return false;
  return true;
}

void UpdatePlayerPos(Player* player, Camera* camera) {
    player->position.x = camera->position.x;
    player->position.z = camera->position.z;
    
    float newY = player->position.y + 2;
    float dy = newY - camera->position.y;
    camera->position.y = newY;
    camera->target.y += dy;
}

int main(void)
{
    Player player;
    player.position = (Vector3) { 0.0f, 50.0f, 2.0f};
    player.velocity = (Vector3) { 0.0f, 0.0f, 0.0f};
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minecraft");
    //INIT CAMERA
    
    Camera3D camera = { 0 };
    camera.position   = (Vector3){ player.position.x, player.position.y+2, player.position.z};
    camera.target     = (Vector3){ 2.0f, 0.0f, 2.0f };
    camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy       = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    //un objet bloc peut etre avec name id etc
    for(int x = 0; x < X_SIZE; x++) {
        for(int y = 0; y < Y_SIZE; y++) {
            for(int z = 0; z < Z_SIZE; z++) {
                if( y == 49 && x <= 16 && z <= 16) {
                    map[x][y][z] = GRASS;
                } else if(y < 49 && y >= 40 && x <= 16 && z <= 16) {
                    map[x][y][z] = DIRT;
                } else if(y < 40 && y > 0 && x <= 16 && z <= 16) {
                    map[x][y][z] = STONE;
                } else {
                    map[x][y][z] = AIR;
                }
           }
        }
    }
   
    Texture2D texture[3];
    
    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model model = LoadModelFromMesh(cube);
    
    texture[0] = LoadTexture("resources/grass.png");
    texture[1] = LoadTexture("resources/dirt.png");
    texture[2] = LoadTexture("resources/stone.png");
    
    for(int i = 0; i < 3; i++) SetTextureFilter(texture[i], TEXTURE_FILTER_POINT);
    

    DisableCursor();
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {

        float deltaTime = GetFrameTime();
      
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        
        player.position.y += player.velocity.y * deltaTime;
        if(!isOnFloor(player)) {
           if(player.velocity.y > 0) {
               player.velocity.y -= GRAVITY * deltaTime;    
           } else {
               player.velocity.y -= GRAVITY * deltaTime * 2.5f;
           } 
        } else {
            player.velocity.y = 0;
            player.position.y = 50;
        }
        
        
        if(IsKeyPressed(KEY_SPACE) && isOnFloor(player)) {
            player.velocity.y = sqrt(JUMP_HEIGHT * GRAVITY * 2.0f); 
        }
        UpdatePlayerPos(&player, &camera);
        
        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(camera);
                
                for(int x = 0; x < X_SIZE; x++) {
                    for(int y = 0; y < Y_SIZE; y++) {
                        for(int z = 0; z < Z_SIZE; z++) {
                            if(map[x][y][z] != AIR) {
                                model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture[map[x][y][z]];
                                DrawModel(model, (Vector3) {x+ 0.5f, y+ 0.5f, z+ 0.5f}, 1.0f, WHITE);
                            }
                        }
                    }
                }

            EndMode3D();
        
                    
            //CROSSHAIR
            DrawRectangle(SCREEN_WIDTH/2-2,SCREEN_HEIGHT/2-30,4,60,WHITE);
            DrawRectangle(SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2-2,60,4,WHITE);
            
            //COORD
            DrawText(TextFormat("x: %02.02f, y: %02.02f, z: %02.02f", player.position.x, player.position.y, player.position.z), 50, 10, 25, BLACK);
            DrawText(TextFormat("block below the player : %d", getBlock((int) floorf(player.position.x), (int) floorf(player.position.y - 0.05f), (int) floorf(player.position.z))), 50, 35, 25, BLACK);
            //DrawText(TextFormat("is on floor : %d", isOnFloor(player.position)), 50, 40, 25, BLACK);
            DrawFPS(1200, 10);
           
        EndDrawing();
        
    }
    
    UnloadModel(model);
    for(int i = 0; i < 3; i++) {
        UnloadTexture(texture[i]);
    }
    
    CloseWindow();
    return 0;
}
