#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>

#define GRAVITY -15
#define JUMP_HEIGHT 2
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define X_SIZE 16
#define Y_SIZE 50
#define Z_SIZE 16

#define REACH 6

#define AIR -1
#define GRASS 0
#define DIRT 1
#define STONE 2

typedef struct {
    int id;
    char* name;
    Texture texture;
} Bloc;

static int map[X_SIZE][Y_SIZE][Z_SIZE];

bool isOnFloor(Camera camera) {
  if(camera.position.y > 52) {
    return false;
  }
  return true;
}

int main(void)
{
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minecraft");
    //INIT CAMERA

    
    //Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
    
    Camera3D camera = { 0 };
    camera.position   = (Vector3){ 0.0f, 53.0f, 2.0f};
    camera.target     = (Vector3){ 2.0f, 0.0f, 2.0f };
    camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy       = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    //un objet bloc peut etre avec name id etc
    Vector3 playerPosition = { camera.position.x, camera.position.y-2, camera.position.z };
    for(int x = 0; x < X_SIZE; x++) {
        for(int y = 0; y < Y_SIZE; y++) {
            for(int z = 0; z < Z_SIZE; z++) {
                int val = rand() % 2;
                if( y == 49 ) {
                    map[x][y][z] = GRASS;
                } else if(y < 50 && y >= 40 ) {
                    map[x][y][z] = DIRT;
                } else if(y < 40 && y > 0 ) {
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
    BoundingBox PlayerBox;    
    

    DisableCursor();

    while (!WindowShouldClose())
    {

        SetTargetFPS(60);
        float deltaTime = GetFrameTime();
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        
        //direction - reach en gros (avec des vecteurs)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            //map[bx][by][bz] = AIR;
        
   
        if(!isOnFloor(camera)) {    
           camera.position.y += deltaTime*GRAVITY; 
           camera.target.y += deltaTime*GRAVITY;
        }
        
        if(IsKeyPressed(KEY_SPACE) && isOnFloor(camera)) {
            
            camera.position.y += JUMP_HEIGHT;
            camera.target.y += JUMP_HEIGHT;
           
          //mouvement animation tant que pas +=2
        }
        
        BeginDrawing();
            ClearBackground(SKYBLUE);
            BeginMode3D(camera);
                
                for(int x = 0; x < X_SIZE; x++) {
                    for(int y = 0; y < Y_SIZE; y++) {
                        for(int z = 0; z < Z_SIZE; z++) {
                            if(map[x][y][z] != AIR) {
                                model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture[map[x][y][z]];
                                DrawModel(model, (Vector3) {x, y, z}, 1.0f, WHITE);
                            }
                        }
                    }
                }

            EndMode3D();
        
                    
            //CROSSHAIR
            DrawRectangle(SCREEN_WIDTH/2-2,SCREEN_HEIGHT/2-30,4,60,WHITE);
            DrawRectangle(SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2-2,60,4,WHITE);
            
            //COORD
            DrawText(TextFormat("x: %02.02f, y: %02.02f, z: %02.02f", camera.position.x, camera.position.y, camera.position.z), 50, 10, 25, BLACK);
           
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
