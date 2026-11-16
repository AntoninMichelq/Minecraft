#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define SIZEMAP 100
#define GRAVITY -15
#define JUMP_HEIGHT 2
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// verifier la collision on va utiliser un bool qui va etre activer si le joueur possede les coords du blocks au sols on verifie si y-2 du joueur si il y a un block si il n'y en a pas on applique la gravity 
//utiliser la fonction noise pour generer une map procedurale puis tableau de mesh bloc global et tableau de texture
// TABLEAU DE INT POUR ENSUITE TOUT GENERER D UN COUP
void loadMap(Model* load_tab, size_t size, Texture2D* texture, Mesh mesh_cube) {
    // grass 
    for(size_t i = 0; i < size; i++) { 
        for(size_t j = 0; j < size; j++) { 
            load_tab[i * size + j] = LoadModelFromMesh(mesh_cube);
            load_tab[i * size + j].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture[0];
        }
    }
}

void unloadMap(Model* load_tab, size_t size) {
    for(size_t i = 0; i < size; i++) { 
        for(size_t j = 0; j < size; j++) { 
            UnloadModel(load_tab[i * size + j]);
        }
    }

}


int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minecraft");
    //INIT CAMERA
    Camera3D camera = { 0 };
    camera.position   = (Vector3){ 0.0f, 2.0f, 4.0f };
    camera.target     = (Vector3){ 2.0f, 0.0f, 2.0f };
    camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy       = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    Texture2D texture[2];
    texture[0] = LoadTexture("resources/grass.png"); 
    //texture[0] = LoadTexture("resources/dirt.png"); 
    SetTextureFilter(texture[0], TEXTURE_FILTER_POINT);

    Mesh mesh_cube = GenMeshCube(1.0f, 1.0f, 1.0f);

    Model map[SIZEMAP * SIZEMAP];
    
    loadMap(map, SIZEMAP, texture, mesh_cube); 

    SetTargetFPS(60);
    DisableCursor();

    bool isOnFloor() {
      if(camera.position.y > 2) {
        return false;
      }
      return true;
    }

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        
        if(!isOnFloor()) {         
           camera.position.y += deltaTime*GRAVITY; 
           camera.target.y += deltaTime*GRAVITY;
        }
        if(IsKeyPressed(KEY_SPACE) && isOnFloor()) {
          camera.position.y += JUMP_HEIGHT;
          camera.target.y += JUMP_HEIGHT;
          //mouvement animation tant que pas +=2
        }
        BeginDrawing();
            ClearBackground(SKYBLUE);
 
            BeginMode3D(camera);
            
                for(int i = 0; i < SIZEMAP; i++) { 
                  for(int j = 0; j < SIZEMAP; j++) { 
                    DrawModel(map[i * SIZEMAP + j], (Vector3){ -1.0f*j, 0.0f, 0.0f*j+i }, 1.0f, WHITE);
                  }
                }

            EndMode3D();

            //CROSSHAIR
            DrawRectangle(SCREEN_WIDTH/2-3,SCREEN_HEIGHT/2-30,6,60,WHITE);
            DrawRectangle(SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2-3,60,6,WHITE);
            //COORD
            DrawText(TextFormat("x: %02.02f, y: %02.02f, z: %02.02f", camera.position.x, camera.position.y, camera.position.z), 50, 10, 25, BLACK);

        EndDrawing();
    }
    
    unloadMap(map, SIZEMAP);
    UnloadTexture(texture[0]);
    CloseWindow();
    return 0;
}
