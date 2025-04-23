
#include "raylib.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FRAMES 10

typedef struct {
    Texture2D frames[MAX_FRAMES];
    int frameCount;
    float frameDuration;
} Animation;

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Player Animado");

    FILE *file = fopen("assets/sprites/data/player.json", "rb");
    if (!file) {
        printf("Erro ao abrir player.json\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);
    char *jsonData = malloc(len + 1);
    fread(jsonData, 1, len, file);
    jsonData[len] = '\0';
    fclose(file);

    cJSON *root = cJSON_Parse(jsonData);
    cJSON *animations = cJSON_GetObjectItemCaseSensitive(root, "walk_animation");
    cJSON *directions[4] = {
        cJSON_GetObjectItemCaseSensitive(animations, "down"),
        cJSON_GetObjectItemCaseSensitive(animations, "up"),
        cJSON_GetObjectItemCaseSensitive(animations, "right"),
        cJSON_GetObjectItemCaseSensitive(animations, "left")
    };

    const char *directionNames[4] = { "down", "up", "right", "left" };
    Animation anims[4];
    float frameDuration = (float)cJSON_GetObjectItemCaseSensitive(root, "frame_duration")->valuedouble;

    for (int d = 0; d < 4; d++) {
        int count = cJSON_GetArraySize(directions[d]);
        anims[d].frameCount = count;
        anims[d].frameDuration = frameDuration;
        for (int i = 0; i < count; i++) {
            const char *filename = cJSON_GetArrayItem(directions[d], i)->valuestring;
            char fullpath[256];
            sprintf(fullpath, "assets/sprites/player_sheet/player_walk/%s", filename);
            anims[d].frames[i] = LoadTexture(fullpath);
        }
    }

    free(jsonData);
    cJSON_Delete(root);

    Vector2 pos = { screenWidth / 2.0f, screenHeight / 2.0f };
    int dir = 0;
    int currentFrame = 0;
    float frameTime = 0.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        bool isMoving = false;

        if (IsKeyDown(KEY_W)) { pos.y -= 2.0f; dir = 1; isMoving = true; }
        else if (IsKeyDown(KEY_S)) { pos.y += 2.0f; dir = 0; isMoving = true; }
        else if (IsKeyDown(KEY_D)) { pos.x += 2.0f; dir = 2; isMoving = true; }
        else if (IsKeyDown(KEY_A)) { pos.x -= 2.0f; dir = 3; isMoving = true; }

        if (isMoving) {
            frameTime += GetFrameTime();
            if (frameTime >= anims[dir].frameDuration) {
                frameTime = 0;
                currentFrame = (currentFrame + 1) % anims[dir].frameCount;
            }
        } else {
            currentFrame = 0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Texture2D frame = anims[dir].frames[currentFrame];
        Vector2 origin = { frame.width / 2.0f, frame.height };
        DrawTexturePro(frame, (Rectangle){0, 0, frame.width, frame.height},
                       (Rectangle){pos.x, pos.y, frame.width, frame.height},
                       origin, 0.0f, WHITE);

        EndDrawing();
    }

    for (int d = 0; d < 4; d++) {
        for (int i = 0; i < anims[d].frameCount; i++) {
            UnloadTexture(anims[d].frames[i]);
        }
    }

    CloseWindow();
    return 0;
}
