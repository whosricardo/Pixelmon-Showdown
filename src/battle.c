#include "battle.h"
#include "battle_screen.h"
#include "team.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

void StartBattle(TeamNode *team)
{
    // Initialize the game window
    InitWindow(1280, 720, "Pixelmon Showdown - Battle");
    SetTargetFPS(60);

    // Initialize the battle screen with the selected team
    InitBattleScreen(team);

    // Main battle loop
    while (!WindowShouldClose())
    {
        UpdateBattleScreen();  // Handle player input
        DrawBattleScreen();    // Render the battle screen
    }

    // Clean up
    UnloadBattleScreen();
    CloseWindow();
}
