#include "raylib.h"
#include "tile.h"
#include "map.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Teste de Mapa");

    // Inicializa os tiles carregando cada imagem
    init_tile_definitions();

    // Inicializa o mapa (preenche com grama + árvores)
    init_mapa();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha o mapa
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int tile_id = mapa[y][x];
                TileDefinition *tile = get_tile_by_id(tile_id);
                if (tile) {
                    Vector2 pos = { x * TILE_SIZE, y * TILE_SIZE };
                    DrawTexture(tile->texture, pos.x, pos.y, WHITE);
                }
            }
        }

        EndDrawing();
    }

    // Libera a memória dos tiles
    unload_tile_definitions();
    CloseWindow();

    return 0;
}
