#include "raylib.h"
#include "menu.h"
#include "battle_screen.h"

int main(void)
{
    InitWindow(1280, 720, "Pixelmon Showdown");
    SetTargetFPS(60);

    // Load the move data 
    LoadMoveData();
    LoadTypeChart();

    // Show the main menu
    ShowMainMenu();

    // Free the move data when exiting
    FreeMoveData();
    FreeTypeChart();

    CloseWindow();
    return 0;
}
