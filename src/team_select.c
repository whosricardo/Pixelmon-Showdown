#include "team_select.h"
#include "pokemon_loader.h"
#include "file_loader.h"
#include "team.h"
#include "battle_screen.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_ROWS 5
#define GRID_COLS 6
#define MAX_TEAM_SIZE 6
#define MAX_POKEMON 300
#define TEAM_PREVIEW_X 360
#define TEAM_PREVIEW_Y 620
#define TEAM_PREVIEW_SPACING 80

static Texture2D background;
static PokemonIcon icons[MAX_POKEMON]; 
static int total_pokemon = 0;
static TeamNode *player_team = NULL;
static Texture2D cursor_texture;
static float cursor_offset = 0;
static int cursor_direction = 1;
static float cursor_speed = 0.3f;

static void LoadPokemonIcons()
{
    const char *generations[] = {"gen1", "gen2", "gen3"};
    total_pokemon = 0;

    // Loop through each generation folder
    for (int g = 0; g < 3; g++)
    {
        char gen_path[256];
        snprintf(gen_path, sizeof(gen_path), "assets/sprites/pokemons/%s", generations[g]);

        DIR *dir = opendir(gen_path);
        if (dir == NULL)
        {
            fprintf(stderr, "ERROR: Failed to open directory %s.\n", gen_path);
            continue;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.') continue;

            // Build the full path to the JSON file
            char json_path[256];
            snprintf(json_path, sizeof(json_path), "%s/%s/%s.json", gen_path, entry->d_name, entry->d_name);

            // Load the JSON to check the evolution status
            PokemonInfo *pokemon = load_pokemon_info(json_path);
            if (pokemon == NULL) continue;

            // Load the icon for every Pokémon, regardless of evolution stage
            char icon_path[256];
            snprintf(icon_path, sizeof(icon_path), "%s/%s/%s_icon.png", gen_path, entry->d_name, entry->d_name);
            if (FileExists(icon_path))
            {
                icons[total_pokemon].texture = LoadTexture(icon_path);
                strncpy(icons[total_pokemon].name, entry->d_name, sizeof(icons[total_pokemon].name) - 1);
                strncpy(icons[total_pokemon].generation, generations[g], sizeof(icons[total_pokemon].generation) - 1);
                icons[total_pokemon].is_legendary = pokemon->is_legendary;
                total_pokemon++;
            }

            free_pokemon(pokemon);

            if (total_pokemon >= MAX_POKEMON) break;
        }

        closedir(dir);
    }

    printf("Loaded %d Pokémon icons.\n", total_pokemon);
}

void ShowTeamSelection()
{
    // Load the background image
    Image bgImage = LoadImage("assets/background/team_select.jpeg");
    ImageResize(&bgImage, 1280, 720);
    background = LoadTextureFromImage(bgImage);
    UnloadImage(bgImage);

    // Load cursor
    cursor_texture = LoadTexture("assets/misc/selector.png");

    // Load the Pokémon icons
    LoadPokemonIcons();

    int selected_row = 0;
    int selected_col = 0;
    int current_page = 0;
    int total_pages = (total_pokemon + (GRID_ROWS * GRID_COLS) - 1) / (GRID_ROWS * GRID_COLS);
    int selected_index = 0;

    // Main team selection loop
    while (!WindowShouldClose())
    {
        // Handle grid navigation
        if (IsKeyPressed(KEY_DOWN)) selected_row = (selected_row + 1) % GRID_ROWS;
        if (IsKeyPressed(KEY_UP)) selected_row = (selected_row - 1 + GRID_ROWS) % GRID_ROWS;
        if (IsKeyPressed(KEY_RIGHT)) selected_col = (selected_col + 1) % GRID_COLS;
        if (IsKeyPressed(KEY_LEFT)) selected_col = (selected_col - 1 + GRID_COLS) % GRID_COLS;

        // Handle page switching
        if (IsKeyPressed(KEY_Q) && current_page > 0)
        {
            current_page--;
            selected_row = 0;
            selected_col = 0;
        }
        else if (IsKeyPressed(KEY_E) && current_page < total_pages - 1)
        {
            current_page++;
            selected_row = 0;
            selected_col = 0;
        }

        // Calculate the selected index
        selected_index = current_page * (GRID_ROWS * GRID_COLS) + (selected_row * GRID_COLS) + selected_col;
        if (selected_index >= total_pokemon) selected_index = total_pokemon - 1;
        if (selected_index < 0) selected_index = 0;

        // Handle Pokémon selection (using Enter or Space)
        if (IsKeyPressed(KEY_SPACE))
        {
            if (team_size(player_team) < MAX_TEAM_SIZE && !is_pokemon_in_team(player_team, selected_index))
            {
                // Prevent more than 1 legendary
                int legendary_count = 0;
                TeamNode *current = player_team;
                while (current != NULL)
                {
                    if (current->pokemon->is_legendary) legendary_count++;
                    current = current->next;
                }

                if (icons[selected_index].is_legendary && legendary_count >= 1)
                {
                    printf("You already have a legendary Pokémon in your team!\n");
                }
                else
                {
                    // Load the Pokémon info
                    char pokemon_json_path[256];
                    snprintf(pokemon_json_path, sizeof(pokemon_json_path), "assets/sprites/pokemons/%s/%s/%s.json",
                            icons[selected_index].generation, icons[selected_index].name, icons[selected_index].name);

                    PokemonInfo *selected_pokemon = load_pokemon_info(pokemon_json_path);
                    if (selected_pokemon)
                    {
                        selected_pokemon->is_legendary = icons[selected_index].is_legendary;
                        add_to_team(&player_team, selected_index, selected_pokemon);
                        print_team(player_team);  // For debugging
                    }
                }
            }
        }

        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw the background
        DrawTexture(background, 0, 0, WHITE);
        
        // Draw the Pokémon icons in the matrix
        int index = current_page * (GRID_ROWS * GRID_COLS);
        int matrix_x_offset = (1280 - (GRID_COLS * 120 - 56)) / 2;
        int matrix_y_offset = 100;

        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                if (index >= total_pokemon) break;

                int cell_x = matrix_x_offset + col * 120;
                int cell_y = matrix_y_offset + row * 120;
                int icon_x = cell_x + (64 - icons[index].texture.width) / 2;
                int icon_y = cell_y + (64 - icons[index].texture.height) / 2;
                
                DrawTexture(icons[index].texture, icon_x, icon_y, WHITE);

                // Draw the cursor
                if (index == selected_index)
                {
                    cursor_offset += cursor_direction * cursor_speed;
                    if (cursor_offset > 5 || cursor_offset < -5)
                    {
                        cursor_direction *= -1;
                    }

                    int cursor_x = cell_x + 50;
                    int cursor_y = cell_y + (10 - cursor_texture.height) / 2 + (int)cursor_offset;
                    DrawTexture(cursor_texture, cursor_x, cursor_y, WHITE);
                }

                index++;
            }
        }

        // Draw page indicator
        char page_text[32];
        snprintf(page_text, sizeof(page_text), "Page %d / %d", current_page + 1, total_pages);
        DrawText(page_text, 1100, 680, 20, WHITE);

        // Draw the team preview
        int current_team_size = team_size(player_team);
        int total_preview_width = (current_team_size * TEAM_PREVIEW_SPACING) - (TEAM_PREVIEW_SPACING - 64);
        int preview_x = (1280 - total_preview_width) / 2;  // Center the team

        TeamNode *current = player_team;
        while (current != NULL)
        {
            for (int i = 0; i < total_pokemon; i++)
            {
                if (strcasecmp(current->pokemon->name, icons[i].name) == 0)
                {
                    DrawTexture(icons[i].texture, preview_x + 15, TEAM_PREVIEW_Y + 12, WHITE);
                    preview_x += TEAM_PREVIEW_SPACING;
                    break;
                }
            }
            current = current->next;
        }

        // Draw the confirmation button
        int button_x = 500;
        int button_y = 50;
        int button_width = 280;
        int button_height = 50;
        Color button_color = (team_size(player_team) == MAX_TEAM_SIZE) ? GREEN : GRAY;
        DrawRectangle(button_x, button_y, button_width, button_height, button_color);
        DrawText("CONFIRM TEAM", button_x + 60, button_y + 15, 20, WHITE);

        // Handle the confirmation button click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse_pos = GetMousePosition();
            if (mouse_pos.x >= button_x && mouse_pos.x <= button_x + button_width &&
                mouse_pos.y >= button_y && mouse_pos.y <= button_y + button_height)
            {
                if (team_size(player_team) == MAX_TEAM_SIZE)
                {
                    printf("Team Confirmed!\n");
                    
                    // Call the battle screen with the selected team
                    InitBattleScreen(player_team);
                    
                    // Break out of the selection loop
                    break;
                }
                else
                {
                    printf("You must select 6 Pokémon to confirm your team!\n");
                }
            }
        }

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(cursor_texture);
    for (int i = 0; i < total_pokemon; i++)
    {
        UnloadTexture(icons[i].texture);
    }
    free_team(player_team);
}
