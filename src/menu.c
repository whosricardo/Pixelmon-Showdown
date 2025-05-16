#include "raylib.h"
#include "menu.h"
#include "team_select.h"

static Texture2D background;

void ShowMainMenu()
{
    // Load the background image
    Image bgImage = LoadImage("assets/background/background.png");
    ImageResize(&bgImage, 1280, 720);
    background = LoadTextureFromImage(bgImage);
    UnloadImage(bgImage);

    // Menu state
    const int menu_options = 2;
    int selected_option = 0;

    // Main menu loop
    while (!WindowShouldClose())
    {
        // Handle input
        if (IsKeyPressed(KEY_DOWN)) selected_option = (selected_option + 1) % menu_options;
        if (IsKeyPressed(KEY_UP)) selected_option = (selected_option - 1 + menu_options) % menu_options;
        
        if (IsKeyPressed(KEY_ENTER))
        {
            if (selected_option == 0) 
            {
                ShowTeamSelection();
                break;
            }
            else if (selected_option == 1) 
            {
                CloseWindow(); // Exit the game
                return;
            }
        }

        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw the resized background
        DrawTexture(background, 0, 0, WHITE);
        
        // Draw the menu options
        DrawText("PLAY", 500, 600, 40, selected_option == 0 ? YELLOW : WHITE);
        DrawText("EXIT", 675, 600, 40, selected_option == 1 ? YELLOW : WHITE);
        
        EndDrawing();
    }

    // Unload the background when done
    UnloadTexture(background);
}
