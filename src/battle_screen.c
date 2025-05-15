#include "raylib.h"
#include "player_loader.h"
#include "rival_loader.h"
#include "team.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define BATTLE_SPRITE_SCALE 3
#define HEALTH_BAR_WIDTH 200
#define HEALTH_BAR_HEIGHT 20
#define ATTACK_BUTTON_WIDTH 250
#define ATTACK_BUTTON_HEIGHT 40
#define ATTACK_BUTTON_SPACING 20
#define TEAM_PREVIEW_Y 620
#define TEAM_PREVIEW_SPACING 100
#define MAX_TEAM_SIZE 6
#define MAX_POKEMON 300

static Texture2D background;
static Player player;
static Rival rival;
static TeamNode *player_team = NULL;
static int selected_attack = 0;
static int player_health = 100;
static int rival_health = 100;
static Texture2D player_pokemon_texture;
static int is_player_turn = 1;
static int battle_ended = 0;

typedef struct {
    Texture2D texture;
    char name[64];
    int is_legendary;
} PokemonIcon;

static PokemonIcon icons[MAX_POKEMON];
static int total_pokemon = 0;

const char *attack_names[] = {"Tackle", "Ember", "Quick Attack", "Thunderbolt"};

void LoadPokemonIcons()
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

void InitBattleScreen(TeamNode *team)
{
    // Load background and resize to fit the window
    Image bgImage = LoadImage("assets/background/battle_bg.png");
    ImageResize(&bgImage, SCREEN_WIDTH, SCREEN_HEIGHT);
    background = LoadTextureFromImage(bgImage);
    UnloadImage(bgImage);

    // Load player and rival sprites
    load_player_sprite(&player, "assets/sprites/player/player.json");
    load_rival_sprite(&rival, "assets/sprites/rival/rival.json");

    // Load Pokémon icons (reused from team_select.c)
    LoadPokemonIcons();

    // Set the player team
    player_team = team;

    // Load the front texture for the first Pokémon in the team
    if (player_team != NULL)
    {
        PokemonInfo *first_pokemon = player_team->pokemon;
        
        // Search through each generation to find the correct sprite
        const char *generations[] = {"gen1", "gen2", "gen3"};
        char front_path[256] = "";
        for (int g = 0; g < 3; g++)
        {
            snprintf(front_path, sizeof(front_path), "assets/sprites/pokemons/%s/%s/%s", 
                     generations[g], first_pokemon->name, first_pokemon->sprites.front);
            
            if (FileExists(front_path))
            {
                player_pokemon_texture = LoadTexture(front_path);
                printf("Loaded player Pokémon front sprite: %s\n", front_path);
                break;
            }
        }

        if (player_pokemon_texture.id == 0)
        {
            printf("ERROR: Failed to load front sprite for %s\n", first_pokemon->name);
        }
    }
}

void DrawBattleScreen()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // Draw the background
    DrawTexture(background, 0, 0, WHITE);
    
    // Position for the player (left side)
    int player_x = 150;
    int player_y = SCREEN_HEIGHT - player.battle.height * BATTLE_SPRITE_SCALE - 150;
    
    // Position for the rival (right side)
    int rival_x = SCREEN_WIDTH - rival.battle.width * BATTLE_SPRITE_SCALE - 150;
    int rival_y = 100;
    
    // Draw player and rival sprites
    DrawTextureEx(player.battle, (Vector2){player_x, player_y}, 0.0f, BATTLE_SPRITE_SCALE, WHITE);
    DrawTextureEx(rival.battle, (Vector2){rival_x, rival_y}, 0.0f, BATTLE_SPRITE_SCALE, WHITE);

    // Draw health bars
    DrawRectangle(player_x - 55, player_y - 30, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, DARKGREEN);
    DrawRectangle(rival_x, rival_y - 30, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, DARKGREEN);
    DrawRectangleLines(player_x - 55, player_y - 30, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, BLACK);
    DrawRectangleLines(rival_x, rival_y - 30, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, BLACK);

    // Draw player Pokémon
    if (player_team != NULL && player_pokemon_texture.id > 0)
    {
        int pokemon_x = player_x + 250;
        int pokemon_y = player_y - player_pokemon_texture.height + 50;
        DrawTextureEx(player_pokemon_texture, (Vector2){pokemon_x, pokemon_y}, 0.0f, 2.0f, WHITE);
    }

    // Draw attack menu
    int button_x = 80;
    int button_y = 600;
    for (int i = 0; i < 4; i++)
    {
        Color button_color = (i == selected_attack) ? ORANGE : GRAY;
        DrawRectangle(button_x, button_y, ATTACK_BUTTON_WIDTH, ATTACK_BUTTON_HEIGHT, button_color);
        DrawText(attack_names[i], button_x + 20, button_y + 10, 20, WHITE);
        button_x += ATTACK_BUTTON_WIDTH + ATTACK_BUTTON_SPACING;
    }

    // Draw team preview
    int preview_x = (SCREEN_WIDTH - (TEAM_PREVIEW_SPACING * MAX_TEAM_SIZE)) / 2;
    TeamNode *current = player_team;
    while (current != NULL)
    {
        for (int i = 0; i < total_pokemon; i++)
        {
            if (strcasecmp(current->pokemon->name, icons[i].name) == 0)
            {
                // Scale the icons for better visibility
                DrawTextureEx(icons[i].texture, (Vector2){preview_x, TEAM_PREVIEW_Y + 35}, 0.0f, 3.0f, WHITE);
                preview_x += TEAM_PREVIEW_SPACING;
                break;
            }
        }
        current = current->next;
    }

    EndDrawing();
}

void UpdateBattleScreen()
{
    if (battle_ended) return;

    // Handle attack selection
    if (IsKeyPressed(KEY_RIGHT)) selected_attack = (selected_attack + 1) % 4;
    if (IsKeyPressed(KEY_LEFT)) selected_attack = (selected_attack - 1 + 4) % 4;

    // Handle attack execution
    if (IsKeyPressed(KEY_ENTER))
    {
        printf("Selected Attack: %s\n", attack_names[selected_attack]);

        // Apply player attack
        int player_damage = 20 + (rand() % 10);  // Random damage between 20 and 30
        rival_health -= player_damage;
        printf("Player dealt %d damage!\n", player_damage);

        // Check if the rival is defeated
        if (rival_health <= 0)
        {
            rival_health = 0;
            printf("You won the battle!\n");
            battle_ended = 1;
            return;
        }

        // Apply rival attack (simple counterattack logic)
        int rival_damage = 15 + (rand() % 15);  // Random damage between 15 and 30
        player_health -= rival_damage;
        printf("Rival dealt %d damage!\n", rival_damage);

        // Check if the player is defeated
        if (player_health <= 0)
        {
            player_health = 0;
            printf("You lost the battle!\n");
            battle_ended = 1;
            return;
        }

        // Toggle turn
        is_player_turn = !is_player_turn;
    }
}

void UnloadBattleScreen()
{
    UnloadTexture(background);
    free_player(&player);
    free_rival(&rival);
    UnloadTexture(player_pokemon_texture);

    // Unload all Pokémon icons
    for (int i = 0; i < total_pokemon; i++)
    {
        UnloadTexture(icons[i].texture);
    }
}
