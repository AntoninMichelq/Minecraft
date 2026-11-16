#include "raylib.h"

int main(void)
{
    InitWindow(800, 600, "Cube texture");

    Camera3D camera = { 0 };
    camera.position   = (Vector3){ 6.0f, 6.0f, 6.0f };
    camera.target     = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up         = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;


    Texture2D texture = LoadTexture("grass.png");
    SetTextureFilter(texture, TEXTURE_FILTER_POINT); 


    Mesh mesh = GenMeshCube(2.0f, 4.0f, 2.0f);
    Model cube = LoadModelFromMesh(mesh);
    cube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawGrid(10, 1.0f);
                DrawModel(cube, (Vector3){ -2.0f, 2.0f, 0.0f }, 1.0f, WHITE);
            EndMode3D();
        EndDrawing();
    }

    UnloadModel(cube);     
    UnloadTexture(texture);
    CloseWindow();
    return 0;
}
