#include "team_select.h"
#include "pokemon_loader.h"
#include "file_loader.h"
#include "team.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_ROWS 5
#define GRID_COLS 6
#define MAX_TEAM_SIZE 6
#define MAX_POKEMON 300

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

            // Load only first-stage or single-stage Pokémon
            if ((pokemon->pre_evolution == NULL && pokemon->evolves_to != NULL) || 
                (pokemon->pre_evolution == NULL && pokemon->evolves_to == NULL))
            {
                // Load the icon
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
            }

            free_pokemon(pokemon);

            // Stop if we reach the max limit
            if (total_pokemon >= MAX_POKEMON) break;
        }

        closedir(dir);
    }

    printf("Loaded %d first-stage and single-stage Pokémon icons.\n", total_pokemon);
}

void ShowTeamSelection()
{
    // Load the background image
    Image bgImage = LoadImage("assets/background/team_select.jpeg");
    ImageResize(&bgImage, 1280, 720);
    background = LoadTextureFromImage(bgImage);
    UnloadImage(bgImage);

    // load cursor
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

        // Calculate the selected index
        selected_index = current_page * (GRID_ROWS * GRID_COLS) + (selected_row * GRID_COLS) + selected_col;

        // Prevent out-of-bounds selection
        if (selected_index >= total_pokemon) selected_index = total_pokemon - 1;
        if (selected_index < 0) selected_index = 0;

        // Handle page switching
        if (IsKeyPressed(KEY_RIGHT) && current_page < total_pages - 1)
        {
            current_page++;
            selected_row = 0;
            selected_col = 0;
        }

        if (IsKeyPressed(KEY_LEFT) && current_page > 0)
        {
            current_page--;
            selected_row = 0;
            selected_col = 0;
        }

        // Handle Pokémon selection (using E key)
        if (IsKeyPressed(KEY_E))
        {
            // Prevent adding more than 6 Pokémon
            if (team_size(player_team) < MAX_TEAM_SIZE && !is_pokemon_in_team(player_team, selected_index))
            {
                // Count current legendaries in the team
                int legendary_count = 0;
                TeamNode *current = player_team;
                while (current != NULL)
                {
                    if (current->pokemon->is_legendary) legendary_count++;
                    current = current->next;
                }

                // Prevent selecting more than 1 legendary
                if (icons[selected_index].is_legendary && legendary_count >= 1)
                {
                    printf("You already have a legendary Pokémon in your team!\n");
                }
                else
                {
                    // Build the full path to the Pokémon JSON
                    char pokemon_json_path[256];
                    snprintf(pokemon_json_path, sizeof(pokemon_json_path), "assets/sprites/pokemons/%s/%s/%s.json",
                            icons[selected_index].generation, icons[selected_index].name, icons[selected_index].name);

                    // Load the Pokémon info
                    PokemonInfo *selected_pokemon = load_pokemon_info(pokemon_json_path);
                    if (selected_pokemon)
                    {
                        // Set the correct legendary status
                        selected_pokemon->is_legendary = icons[selected_index].is_legendary;
                        
                        // Check the legendary limit again
                        if (selected_pokemon->is_legendary && legendary_count >= 1)
                        {
                            printf("You already have a legendary Pokémon in your team!\n");
                            free_pokemon(selected_pokemon);
                        }
                        else
                        {
                            // Add the Pokémon to the team
                            add_to_team(&player_team, selected_index, selected_pokemon);
                            print_team(player_team);  // For debugging
                        }
                    }
                }
            }
        }

        // Start drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw the background
        DrawTexture(background, 0, 0, WHITE);
        
        // Draw the Pokémon icons for the current page
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

                // Center the icon within the 64x64 highlight box
                int icon_x = cell_x + (64 - icons[index].texture.width) / 2;
                int icon_y = cell_y + (64 - icons[index].texture.height) / 2;
                
                // Draw the icon
                DrawTexture(icons[index].texture, icon_x, icon_y, WHITE);

                // Draw the hand cursor if this is the selected Pokémon
                if (index == selected_index)
                {
                    // Apply the bouncing effect
                    cursor_offset += cursor_direction * cursor_speed;
                    if (cursor_offset > 5 || cursor_offset < -5)
                    {
                        cursor_direction *= -1;
                    }

                    int cursor_x = cell_x + 50;  // Adjust for better alignment
                    int cursor_y = cell_y + (10 - cursor_texture.height) / 2 + (int)cursor_offset;
                    DrawTexture(cursor_texture, cursor_x, cursor_y, WHITE);
                }

                index++;
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
