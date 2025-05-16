#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include "battle_screen.h"
#include "pokemon_loader.h"
#include "team.h"
#include "raylib.h"
#include <team_select.h>
#include <stdio.h>
#include <string.h>

#define MAX_POKEMON 6
#define MAX_AVAILABLE_POKEMON 300
#define HP_BAR_WIDTH 300

static TeamNode *player_team = NULL;
static TeamNode *rival_team = NULL;
static Texture2D battle_bg;
static Texture2D player_texture;
static Texture2D rival_texture;
static PokemonInfo *player_pokemon = NULL;
static PokemonInfo *rival_pokemon = NULL;
static int player_current_hp = 0;
static int player_max_hp = 0;
static int rival_current_hp = 0;
static int rival_max_hp = 0;

// Generate a random rival team
TeamNode* GenerateRivalTeam()
{
    srand(time(NULL));  // Seed the random number generator

    TeamNode *rival_team = NULL;
    int total_pokemon = 0;
    PokemonInfo *available_pokemon[MAX_AVAILABLE_POKEMON];
    const char *generations[] = {"gen1", "gen2", "gen3"};

    // Load all available Pokémon
    for (int g = 0; g < 3; g++)
    {
        char gen_path[256];
        snprintf(gen_path, sizeof(gen_path), "assets/sprites/pokemons/%s", generations[g]);
        DIR *dir = opendir(gen_path);
        if (!dir) continue;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.') continue;

            // Load the Pokémon JSON
            char json_path[256];
            snprintf(json_path, sizeof(json_path), "%s/%s/%s.json", gen_path, entry->d_name, entry->d_name);
            PokemonInfo *pokemon = load_pokemon_info(json_path);
            if (pokemon == NULL) continue;

            // Store the Pokémon for random selection
            available_pokemon[total_pokemon] = pokemon;
            total_pokemon++;

            if (total_pokemon >= MAX_AVAILABLE_POKEMON) break;
        }

        closedir(dir);
        if (total_pokemon >= MAX_AVAILABLE_POKEMON) break;
    }

    // Select 6 random, unique Pokémon for the rival team
    int team_size = 0;
    while (team_size < MAX_POKEMON)
    {
        int random_index = rand() % total_pokemon;
        PokemonInfo *selected_pokemon = available_pokemon[random_index];

        // Prevent duplicate Pokémon
        if (!is_pokemon_in_team(rival_team, random_index))
        {
            add_to_team(&rival_team, random_index, selected_pokemon);
            team_size++;
        }
    }

    printf("Generated Rival Team:\n");
    TeamNode *current = rival_team;
    while (current != NULL)
    {
        printf("- %s\n", current->pokemon->name);
        current = current->next;
    }

    return rival_team;
}

// Helper function to calculate HP bar width
float CalculateHPBarWidth(int current_hp, int max_hp, int bar_width)
{
    return (float)current_hp / (float)max_hp * bar_width;
}

// Basic damage calculation function
int CalculateDamage(PokemonInfo *attacker, PokemonInfo *defender)
{
    int attack = attacker->base_stats.attack;
    int defense = defender->base_stats.defense;
    int base_power = 40; // Basic power for testing

    // Simple damage formula
    int damage = ((2 * 50 / 5 + 2) * base_power * (attack / (float)defense)) / 50 + 2;
    return damage;
}

// Helper function to get a random move name (for now)
const char* GetRandomMove(PokemonInfo *pokemon)
{
    if (pokemon->learnset_size > 0)
    {
        int random_index = rand() % pokemon->learnset_size;
        return pokemon->learnset[random_index].move;
    }
    return "Tackle";
}

void InitBattleScreen(TeamNode *team)
{
    srand(time(NULL));

    player_team = team;
    rival_team = GenerateRivalTeam();

    // Load the background
    Image bgImage = LoadImage("assets/background/battle_bg.png");
    ImageResize(&bgImage, 1280, 720);
    battle_bg = LoadTextureFromImage(bgImage);
    UnloadImage(bgImage);

    // Get the first Pokémon from the player's team
    player_pokemon = player_team->pokemon;
    player_texture = LoadTexture(player_pokemon->sprites.back);
    player_max_hp = player_pokemon->base_stats.hp * 2;
    player_current_hp = player_max_hp;

    // Get the first Pokémon from the rival's team
    rival_pokemon = rival_team->pokemon;
    rival_texture = LoadTexture(rival_pokemon->sprites.front);
    rival_max_hp = rival_pokemon->base_stats.hp * 2;
    rival_current_hp = rival_max_hp;

    // Temporary: Start the battle loop
    while (!WindowShouldClose())
    {
        // Handle simple battle logic
        if (IsKeyPressed(KEY_SPACE))
        {
            // Player attacks
            int player_damage = CalculateDamage(player_pokemon, rival_pokemon);
            rival_current_hp -= player_damage;
            if (rival_current_hp < 0) rival_current_hp = 0;
            printf("%s used %s! It dealt %d damage.\n", player_pokemon->name, GetRandomMove(player_pokemon), player_damage);

            // Rival attacks
            int rival_damage = CalculateDamage(rival_pokemon, player_pokemon);
            player_current_hp -= rival_damage;
            if (player_current_hp < 0) player_current_hp = 0;
            printf("%s used %s! It dealt %d damage.\n", rival_pokemon->name, GetRandomMove(rival_pokemon), rival_damage);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the background
        DrawTexture(battle_bg, 0, 0, WHITE);

        // Draw the player Pokémon (bottom left)
        DrawTexture(player_texture, 200, 400, WHITE);

        // Draw the rival Pokémon (top right)
        DrawTexture(rival_texture, 800, 100, WHITE);

        // Draw the player HP bar
        int player_bar_width = (int)CalculateHPBarWidth(player_current_hp, player_max_hp, HP_BAR_WIDTH);
        DrawRectangle(150, 550, HP_BAR_WIDTH, 20, DARKGREEN); // Background
        DrawRectangle(150, 550, player_bar_width, 20, GREEN); // Current HP

        // Draw the rival HP bar
        int rival_bar_width = (int)CalculateHPBarWidth(rival_current_hp, rival_max_hp, HP_BAR_WIDTH);
        DrawRectangle(850, 50, HP_BAR_WIDTH, 20, DARKGREEN); // Background
        DrawRectangle(850, 50, rival_bar_width, 20, GREEN); // Current HP

        // Draw player and rival names
        DrawText(player_pokemon->name, 150, 520, 20, WHITE);
        DrawText(rival_pokemon->name, 850, 20, 20, WHITE);

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(battle_bg);
    UnloadTexture(player_texture);
    UnloadTexture(rival_texture);
}