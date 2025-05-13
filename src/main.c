#include "raylib.h"
#include "menu.h"

int main(void)
{
    // Initialize the window
    InitWindow(1280, 720, "Pixelmon Showdown");
    SetTargetFPS(60);

    // Show the main menu
    ShowMainMenu();

    // Close the window when the game exits
    CloseWindow();
    return 0;
}
