#include "raylib.h"
#include "menu.h"
#include "battle_screen.h"

int main(void)
{
    // Initialize the window
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

    // Close the window when the game exits
    CloseWindow();
    return 0;
}
