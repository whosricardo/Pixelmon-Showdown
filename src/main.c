#include "raylib.h"
#include "player.h"
#include "player_loader.h"

int main(void) {
    InitWindow(800, 600, "PixelmonQuest");
    SetTargetFPS(60);

    Player player = {
        .position = {100, 100},
        .direction = DIRECTION_DOWN,
        .mode = MODE_WALK,
        .current_frame = 1,
        .frame_timer = 0,
        .animations = load_player_animations("assets/sprites/player/player.json")
    };

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        update_player(&player, delta);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_player(&player);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
