#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include "battle_screen.h"
#include "pokemon_loader.h"
#include "team.h"
#include "raylib.h"
#include <stdlib.h>
#include "file_loader.h"
#include "cJSON.h"
#include <team_select.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX_POKEMON 6
#define MAX_AVAILABLE_POKEMON 300
#define HP_BAR_WIDTH 300
#define MENU_X 150
#define MENU_Y 600
#define MENU_WIDTH 400
#define MENU_HEIGHT 120
#define MENU_ITEM_HEIGHT 30

static TeamNode *player_team = NULL;
static TeamNode *rival_team = NULL;
static Texture2D battle_bg;
static Texture2D player_texture;
static Texture2D rival_texture;
static PokemonInfo *player_pokemon = NULL;
static PokemonInfo *rival_pokemon = NULL;
static cJSON *type_chart = NULL;
static cJSON *move_data = NULL;
static int player_current_hp = 0;
static int player_max_hp = 0;
static int rival_current_hp = 0;
static int rival_max_hp = 0;
static int selected_move = 0;

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

// Load move data once
void LoadMoveData()
{
    char *move_data_json = load_file_to_string("assets/move_data.json");
    if (!move_data_json)
    {
        fprintf(stderr, "ERROR: Failed to load move data.\n");
        exit(1);
    }

    move_data = cJSON_Parse(move_data_json);
    free(move_data_json);
    if (!move_data)
    {
        fprintf(stderr, "ERROR: Failed to parse move data.\n");
        exit(1);
    }
}

// Free the move data when the game closes
void FreeMoveData()
{
    if (move_data)
    {
        cJSON_Delete(move_data);
    }
}

// Get the power of a move
int GetMovePower(const char *move_name)
{
    if (!move_data)
    {
        fprintf(stderr, "ERROR: Move data not loaded.\n");
        return 0;
    }

    cJSON *move_item = cJSON_GetObjectItem(move_data, move_name);
    if (cJSON_IsObject(move_item))
    {
        cJSON *power_item = cJSON_GetObjectItem(move_item, "power");
        if (cJSON_IsNumber(power_item))
        {
            printf("Found power for move %s: %d\n", move_name, power_item->valueint);
            return power_item->valueint;
        }
    }

    // If the move is not found, print an error for debugging
    fprintf(stderr, "ERROR: Move not found: %s\n", move_name);
    return 0;
}

// Get the type of a move
const char* GetMoveType(const char *move_name)
{
    cJSON *move_item = cJSON_GetObjectItem(move_data, move_name);
    if (cJSON_IsObject(move_item))
    {
        cJSON *type_item = cJSON_GetObjectItem(move_item, "type");
        if (cJSON_IsString(type_item))
        {
            return type_item->valuestring;
        }
    }
    return "Normal"; // Default type if not found
}

// Helper function to calculate HP bar width
float CalculateHPBarWidth(int current_hp, int max_hp, int bar_width)
{
    return (float)current_hp / (float)max_hp * bar_width;
}

// Calculate the damage for a move
int CalculateDamage(PokemonInfo *attacker, PokemonInfo *defender, const char *move_name)
{
    int base_power = GetMovePower(move_name);
    const char *move_type = GetMoveType(move_name);

    if (base_power == 0)
    {
        printf("%s used %s. It's a status move!\n", attacker->name, move_name);
        return 0;
    }

    int critical = (rand() % 16) == 0 ? 2 : 1;
    int attack = attacker->base_stats.attack;
    int defense = defender->base_stats.defense;

    float type_multiplier = GetTypeEffectiveness(move_type, defender);
    float modifier = critical * type_multiplier * ((rand() % 39 + 217) / 255.0f);

    int damage = (((2 * 50 / 5 + 2) * base_power * (attack / (float)defense)) / 50 + 2) * modifier;

    if (critical == 2)
    {
        printf("It's a critical hit!\n");
    }

    if (type_multiplier > 1.0f)
    {
        printf("It's super effective!\n");
    }
    else if (type_multiplier < 1.0f)
    {
        printf("It's not very effective...\n");
    }

    if (damage < 1) damage = 1;

    printf("Calculated damage: %d\n", damage);
    return damage;
}

// Draw the battle menu
void DrawBattleMenu(PokemonInfo *pokemon)
{
    DrawRectangle(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT, DARKGRAY);
    
    // Only draw moves if the Pokémon has a learnset
    if (pokemon->learnset_size > 0)
    {
        for (int i = 0; i < pokemon->learnset_size; i++)
        {
            Color text_color = (i == selected_move) ? YELLOW : WHITE;
            DrawText(pokemon->learnset[i].move, MENU_X + 20, MENU_Y + 10 + i * MENU_ITEM_HEIGHT, 20, text_color);
        }
    }
    else
    {
        // Fallback message if no moves are available
        DrawText("No moves available!", MENU_X + 20, MENU_Y + 20, 20, RED);
    }
}

// Helper function to get the selected move name
const char* GetSelectedMove(PokemonInfo *pokemon, int selected_index)
{
    if (selected_index < pokemon->learnset_size)
    {
        return pokemon->learnset[selected_index].move;
    }
    return "Struggle";
}

const char* GetRandomMove(PokemonInfo *pokemon)
{
    if (pokemon->learnset_size > 0)
    {
        int random_index = rand() % pokemon->learnset_size;
        return pokemon->learnset[random_index].move;
    }
    return "Struggle";
}

// Load the type chart once
void LoadTypeChart()
{
    char *type_chart_json = load_file_to_string("assets/type_chart.json");
    if (!type_chart_json)
    {
        fprintf(stderr, "ERROR: Failed to load type chart.\n");
        exit(1);
    }

    type_chart = cJSON_Parse(type_chart_json);
    free(type_chart_json);
    if (!type_chart)
    {
        fprintf(stderr, "ERROR: Failed to parse type chart.\n");
        exit(1);
    }
}

// Free the type chart when the game closes
void FreeTypeChart()
{
    if (type_chart)
    {
        cJSON_Delete(type_chart);
    }
}

// Calculate the type effectiveness multiplier
float GetTypeEffectiveness(const char *move_type, PokemonInfo *defender)
{
    if (!type_chart || !move_type || !defender) return 1.0f;

    float multiplier = 1.0f;

    for (int i = 0; i < 2; i++)
    {
        if (defender->type[i] == NULL) continue;

        cJSON *type_info = cJSON_GetObjectItem(type_chart, defender->type[i]);
        if (!cJSON_IsObject(type_info)) continue;

        cJSON *weak_to = cJSON_GetObjectItem(type_info, "weak_to");
        cJSON *resists = cJSON_GetObjectItem(type_info, "resists");

        // Check if the defender is weak to this move
        if (cJSON_IsArray(weak_to))
        {
            int size = cJSON_GetArraySize(weak_to);
            for (int j = 0; j < size; j++)
            {
                cJSON *weakness = cJSON_GetArrayItem(weak_to, j);
                if (cJSON_IsString(weakness) && strcmp(weakness->valuestring, move_type) == 0)
                {
                    multiplier *= 2.0f;
                }
            }
        }

        // Check if the defender resists this move
        if (cJSON_IsArray(resists))
        {
            int size = cJSON_GetArraySize(resists);
            for (int j = 0; j < size; j++)
            {
                cJSON *resist = cJSON_GetArrayItem(resists, j);
                if (cJSON_IsString(resist) && strcmp(resist->valuestring, move_type) == 0)
                {
                    multiplier *= 0.5f;
                }
            }
        }
    }

    return multiplier;
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

    // Main battle loop
    while (!WindowShouldClose())
    {
        // Handle move selection
        if (IsKeyPressed(KEY_DOWN)) selected_move = (selected_move + 1) % player_pokemon->learnset_size;
        if (IsKeyPressed(KEY_UP)) selected_move = (selected_move - 1 + player_pokemon->learnset_size) % player_pokemon->learnset_size;

        // Handle move execution
        if (IsKeyPressed(KEY_ENTER))
        {
            // Player attacks
            int player_damage = CalculateDamage(player_pokemon, rival_pokemon, GetSelectedMove(player_pokemon, selected_move));
            rival_current_hp -= player_damage;
            if (rival_current_hp < 0) rival_current_hp = 0;
            printf("%s used %s! It dealt %d damage.\n", player_pokemon->name, GetSelectedMove(player_pokemon, selected_move), player_damage);

            // Check if the rival Pokémon fainted
            if (rival_current_hp == 0)
            {
                printf("%s fainted!\n", rival_pokemon->name);
                
                // Move to the next Pokémon in the rival's team
                if (rival_team->next != NULL)
                {
                    rival_team = rival_team->next;
                    rival_pokemon = rival_team->pokemon;
                    rival_texture = LoadTexture(rival_pokemon->sprites.front);
                    rival_max_hp = rival_pokemon->base_stats.hp * 2;
                    rival_current_hp = rival_max_hp;
                    printf("Rival sent out %s!\n", rival_pokemon->name);
                }
                else
                {
                    // Rival is out of Pokémon, player wins
                    printf("You win!\n");
                    break;
                }
            }

            // Rival attacks
            int rival_damage = CalculateDamage(rival_pokemon, player_pokemon, GetRandomMove(rival_pokemon));
            player_current_hp -= rival_damage;
            if (player_current_hp < 0) player_current_hp = 0;
            printf("%s used %s! It dealt %d damage.\n", rival_pokemon->name, GetRandomMove(rival_pokemon), rival_damage);

            // Check if the player Pokémon fainted
            if (player_current_hp == 0)
            {
                printf("%s fainted!\n", player_pokemon->name);
                
                // Move to the next Pokémon in the player's team
                if (player_team->next != NULL)
                {
                    player_team = player_team->next;
                    player_pokemon = player_team->pokemon;
                    player_texture = LoadTexture(player_pokemon->sprites.back);
                    player_max_hp = player_pokemon->base_stats.hp * 2;
                    player_current_hp = player_max_hp;
                    printf("You sent out %s!\n", player_pokemon->name);
                }
                else
                {
                    // Player is out of Pokémon, rival wins
                    printf("You lose!\n");
                    break;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the background
        DrawTexture(battle_bg, 0, 0, WHITE);

        // Draw the player Pokémon (bottom left)
        DrawTexture(player_texture, 200, 400, WHITE);

        // Draw the rival Pokémon (top right)
        DrawTexture(rival_texture, 800, 100, WHITE);

        // Draw HP bars
        int player_bar_width = (int)CalculateHPBarWidth(player_current_hp, player_max_hp, HP_BAR_WIDTH);
        int rival_bar_width = (int)CalculateHPBarWidth(rival_current_hp, rival_max_hp, HP_BAR_WIDTH);
        DrawRectangle(150, 550, HP_BAR_WIDTH, 20, DARKGREEN); // Background
        DrawRectangle(150, 550, player_bar_width, 20, GREEN); // Current HP
        DrawRectangle(850, 50, HP_BAR_WIDTH, 20, DARKGREEN); // Background
        DrawRectangle(850, 50, rival_bar_width, 20, GREEN); // Current HP

        // Draw player and rival names
        DrawText(player_pokemon->name, 150, 520, 20, WHITE);
        DrawText(rival_pokemon->name, 850, 20, 20, WHITE);

        // Draw the battle menu
        DrawBattleMenu(player_pokemon);

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(battle_bg);
    UnloadTexture(player_texture);
    UnloadTexture(rival_texture);
}