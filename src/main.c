#include "raylib.h"
#include "player_loader.h"

#define SPRITE_SCALE 3.0f

int main(void) {
    InitWindow(800, 600, "Battle Test");
    SetTargetFPS(60);

    Player player;
    load_player_battle_animation(&player, "assets/sprites/data/player.json");

    int frame = 0;
    float frameTime = 0.0f;
    bool showRival = true;

    while (!WindowShouldClose()) {
        frameTime += GetFrameTime();
        if (frameTime >= player.battle.frameDuration) {
            frameTime = 0;
            frame = (frame + 1) % player.battle.frameCount;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Desenha o rival no início da batalha
        if (showRival) {
            Texture2D rival = player.rival_show;
            Rectangle srcRect = { 0, 0, rival.width, rival.height };
            Rectangle destRect = { 600, 150, rival.width * SPRITE_SCALE, rival.height * SPRITE_SCALE };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(rival, srcRect, destRect, origin, 0.0f, WHITE);
        }

        // Desenha o player
        Texture2D sprite = player.battle.frames[frame];
        Rectangle srcRect = { 0, 0, sprite.width, sprite.height };
        Rectangle destRect = { 100, 150, sprite.width * SPRITE_SCALE, sprite.height * SPRITE_SCALE };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(sprite, srcRect, destRect, origin, 0.0f, WHITE);

        EndDrawing();
    }

    free_player(&player);
    CloseWindow();
    return 0;
}
