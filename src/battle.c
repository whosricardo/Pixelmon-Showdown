#include "battle.h"
#include "team.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Texture2D player_texture;
static Texture2D enemy_texture;
static TeamNode *player_team;
static TeamNode *enemy_team;

void StartBattle(TeamNode *team)
{
    // Set up the window
    InitWindow(1280, 720, "Pixelmon Showdown - Battle");
    SetTargetFPS(60);

    // Set the player team
    player_team = team;

    // Load the first Pokémon from the player's team
    if (player_team && player_team->pokemon)
    {
        char player_sprite_path[256];
        snprintf(player_sprite_path, sizeof(player_sprite_path), "assets/sprites/pokemons/gen1/%s/%s_back.png", 
                 player_team->pokemon->name, player_team->pokemon->name);
        player_texture = LoadTexture(player_sprite_path);
    }

    // For now, just load a random enemy Pokémon
    enemy_team = player_team; // Replace this with actual enemy generation later
    if (enemy_team && enemy_team->pokemon)
    {
        char enemy_sprite_path[256];
        snprintf(enemy_sprite_path, sizeof(enemy_sprite_path), "assets/sprites/pokemons/gen1/%s/%s_front.png", 
                 enemy_team->pokemon->name, enemy_team->pokemon->name);
        enemy_texture = LoadTexture(enemy_sprite_path);
    }

    // Main battle loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the player Pokémon
        if (player_texture.id > 0)
            DrawTexture(player_texture, 200, 400, WHITE);

        // Draw the enemy Pokémon
        if (enemy_texture.id > 0)
            DrawTexture(enemy_texture, 800, 100, WHITE);

        // Draw the HP bars (temporary)
        DrawRectangle(200, 350, 200, 20, RED);
        DrawRectangle(800, 50, 200, 20, RED);

        // Draw the battle text (temporary)
        DrawText("Battle in Progress...", 500, 650, 30, BLACK);

        EndDrawing();
    }

    // Clean up
    if (player_texture.id > 0)
        UnloadTexture(player_texture);

    if (enemy_texture.id > 0)
        UnloadTexture(enemy_texture);

    CloseWindow();
}
