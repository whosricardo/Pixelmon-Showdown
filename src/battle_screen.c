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
static Texture2D burn_icon;
static Texture2D paralyzed_icon;
static Texture2D frozen_icon;
static Texture2D poisoned_icon;
static Texture2D sleep_icon;
static Texture2D confused_icon;
static int player_current_hp = 0;
static int player_max_hp = 0;
static int rival_current_hp = 0;
static int rival_max_hp = 0;
static int selected_move = 0;
static Sound critical_hit_sound;
static int critical_hit_flash_timer = 0;
static const int CRITICAL_HIT_FLASH_DURATION = 15;
static int player_turn = 1;

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

// Apply status effects
void ApplyStatusEffect(PokemonInfo *target, const char *effect)
{
    // Check if the Pokémon already has a status condition
    if (target->status != NULL)
    {
        printf("%s is already affected by %s!\n", target->name, target->status);
        return;
    }

    if (strcmp(effect, "lower_attack") == 0)
    {
        target->base_stats.attack = (int)(target->base_stats.attack * 0.75);
        printf("%s's Attack fell!\n", target->name);
    }
    else if (strcmp(effect, "lower_defense") == 0)
    {
        target->base_stats.defense = (int)(target->base_stats.defense * 0.75);
        printf("%s's Defense fell!\n", target->name);
    }
    else if (strcmp(effect, "paralyze") == 0)
    {
        target->status = strdup("paralyzed");
        printf("%s is paralyzed! It may be unable to move!\n", target->name);
    }
    else if (strcmp(effect, "burn") == 0)
    {
        target->status = strdup("burned");
        printf("%s was burned!\n", target->name);
    }
    else if (strcmp(effect, "poison") == 0)
    {
        target->status = strdup("poisoned");
        printf("%s was poisoned!\n", target->name);
    }
    else if (strcmp(effect, "freeze") == 0)
    {
        target->status = strdup("frozen");
        printf("%s was frozen solid!\n", target->name);
    }
    else if (strcmp(effect, "raise_attack") == 0)
    {
        target->base_stats.attack = (int)(target->base_stats.attack * 1.5);
        printf("%s's Attack sharply rose!\n", target->name);
    }
    else if (strcmp(effect, "raise_sp_attack") == 0)
    {
        target->base_stats.sp_attack = (int)(target->base_stats.sp_attack * 1.5);
        printf("%s's Special Attack sharply rose!\n", target->name);
    }
    else if (strcmp(effect, "heal") == 0)
    {
        int max_hp;
        int heal_amount;

        if (target == player_pokemon)
        {
            max_hp = player_pokemon->base_stats.hp * 2;
            heal_amount = (int)(max_hp * 0.5);  // Heal 50% of max HP
            player_team->hp += heal_amount;
            if (player_team->hp > max_hp) player_team->hp = max_hp;
            player_current_hp = player_team->hp;
            printf("%s recovered %d HP!\n", player_pokemon->name, heal_amount);
        }
        else
        {
            max_hp = rival_pokemon->base_stats.hp * 2;
            heal_amount = (int)(max_hp * 0.5);  // Heal 50% of max HP
            rival_team->hp += heal_amount;
            if (rival_team->hp > max_hp) rival_team->hp = max_hp;
            rival_current_hp = rival_team->hp;
            printf("%s recovered %d HP!\n", rival_pokemon->name, heal_amount);
        }
    }
    else if (strcmp(effect, "protect") == 0)
    {
        // Apply Protect only to the current attacker
        if (player_turn == 1)
        {
            // Player used Protect
            if (player_pokemon->status != NULL && strcmp(player_pokemon->status, "protected") == 0)
            {
                printf("%s is already protected!\n", player_pokemon->name);
                return;
            }

            free(player_pokemon->status);
            player_pokemon->status = strdup("protected");
            printf("%s is protecting itself!\n", player_pokemon->name);
        }
        else
        {
            // Rival used Protect
            if (rival_pokemon->status != NULL && strcmp(rival_pokemon->status, "protected") == 0)
            {
                printf("%s is already protected!\n", rival_pokemon->name);
                return;
            }

            free(rival_pokemon->status);
            rival_pokemon->status = strdup("protected");
            printf("%s is protecting itself!\n", rival_pokemon->name);
        }
    }
}

// Helper function to calculate HP bar width
float CalculateHPBarWidth(int current_hp, int max_hp, int bar_width)
{
    return (float)current_hp / (float)max_hp * bar_width;
}

// Determine HP bar color based on percentage
Color GetHPBarColor(int current_hp, int max_hp)
{
    float hp_percentage = (float)current_hp / max_hp;
    if (hp_percentage > 0.5f)
        return GREEN; // High HP
    else if (hp_percentage > 0.2f)
        return YELLOW; // Medium HP
    else
        return RED; // Low HP
}

// Calculate the damage for a move
int CalculateDamage(PokemonInfo *attacker, PokemonInfo *defender, const char *move_name)
{
    // Prevent damage if the target is protected
    if (defender->status != NULL && strcmp(defender->status, "protected") == 0)
    {
        printf("%s protected itself! No damage taken.\n", defender->name);
        // Clear the protect status after the attack
        free(defender->status);
        defender->status = NULL;
        return 0;
    }

    int base_power = GetMovePower(move_name);
    const char *move_type = GetMoveType(move_name);

    if (base_power == 0)
    {
        // Check for status effects
        cJSON *move_item = cJSON_GetObjectItem(move_data, move_name);
        cJSON *effect_item = cJSON_GetObjectItem(move_item, "effect");

        if (cJSON_IsString(effect_item))
        {
            // Self-buffs and debuffs
            if (strcmp(effect_item->valuestring, "raise_attack") == 0 || 
                strcmp(effect_item->valuestring, "raise_sp_attack") == 0 ||
                strcmp(effect_item->valuestring, "heal") == 0)
            {
                ApplyStatusEffect(attacker, effect_item->valuestring);
            }
            else
            {
                ApplyStatusEffect(defender, effect_item->valuestring);
            }
        }

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
        PlaySound(critical_hit_sound);
        critical_hit_flash_timer = CRITICAL_HIT_FLASH_DURATION;
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

// Draw status icon next to the Pokémon
void DrawStatusIcon(PokemonInfo *pokemon, int x, int y)
{
    if (pokemon->status == NULL) return;

    Texture2D *icon = NULL;
    float status_scale = 1.5f;  // Adjust this for the desired size

    if (strcmp(pokemon->status, "burned") == 0)
        icon = &burn_icon;
    else if (strcmp(pokemon->status, "paralyzed") == 0)
        icon = &paralyzed_icon;
    else if (strcmp(pokemon->status, "frozen") == 0)
        icon = &frozen_icon;
    else if (strcmp(pokemon->status, "poisoned") == 0)
        icon = &poisoned_icon;
    else if (strcmp(pokemon->status, "sleep") == 0)
        icon = &sleep_icon;
    else if (strcmp(pokemon->status, "confused") == 0)
        icon = &confused_icon;

    if (icon)
    {
        // Calculate the scaled position to keep the icon centered
        int icon_width = icon->width * status_scale;
        int icon_height = icon->height * status_scale;
        int icon_x = x - (icon_width / 2);
        int icon_y = y - (icon_height / 2);

        DrawTextureEx(*icon, (Vector2){icon_x, icon_y}, 0.0f, status_scale, WHITE);
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
    // Load the critical hit sound
    InitAudioDevice();
    critical_hit_sound = LoadSound("assets/sounds/critical_hit.mp3");

    srand(time(NULL));

    player_team = team;
    rival_team = GenerateRivalTeam();

    // Load the background
    Image bgImage = LoadImage("assets/background/battle_bg.png");
    ImageResize(&bgImage, 1280, 720);
    battle_bg = LoadTextureFromImage(bgImage);
    UnloadImage(bgImage);

    // Load status icons
    burn_icon = LoadTexture("assets/status/burn.png");
    paralyzed_icon = LoadTexture("assets/status/paralyzed.png");
    frozen_icon = LoadTexture("assets/status/frozen.png");
    poisoned_icon = LoadTexture("assets/status/poisoned.png");
    sleep_icon = LoadTexture("assets/status/sleep.png");
    confused_icon = LoadTexture("assets/status/confused.png");

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

    // Variables for shake effect
    static int player_shake_offset = 0;
    static int rival_shake_offset = 0;
    static int player_shake_timer = 0;
    static int rival_shake_timer = 0;
    static const int SHAKE_DURATION = 15;
    static const int SHAKE_INTENSITY = 10;
    static const int SHAKE_SPEED = 2;

    // Main battle loop
    while (!WindowShouldClose())
    {
        // Update shake timers
        if (player_shake_timer > 0)
        {
            player_shake_timer--;
            if (player_shake_timer % SHAKE_SPEED == 0)
                player_shake_offset = (player_shake_timer / SHAKE_SPEED % 2 == 0) ? -SHAKE_INTENSITY : SHAKE_INTENSITY;
        }
        else
        {
            player_shake_offset = 0;
        }

        if (rival_shake_timer > 0)
        {
            rival_shake_timer--;
            if (rival_shake_timer % SHAKE_SPEED == 0)
                rival_shake_offset = (rival_shake_timer / SHAKE_SPEED % 2 == 0) ? -SHAKE_INTENSITY : SHAKE_INTENSITY;
        }
        else
        {
            rival_shake_offset = 0;
        }

        if (IsKeyPressed(KEY_DOWN)) selected_move = (selected_move + 1) % player_pokemon->learnset_size;
        if (IsKeyPressed(KEY_UP)) selected_move = (selected_move - 1 + player_pokemon->learnset_size) % player_pokemon->learnset_size;

        // Handle move execution
        if (IsKeyPressed(KEY_ENTER))
        {
            if (player_turn == 1)
            {
                // Player attacks
                const char *selected_move_name = GetSelectedMove(player_pokemon, selected_move);
                int player_damage = CalculateDamage(player_pokemon, rival_pokemon, selected_move_name);
                
                // Apply damage to the rival
                rival_team->hp -= player_damage;
                if (rival_team->hp < 0) rival_team->hp = 0;
                rival_current_hp = rival_team->hp;
                rival_shake_timer = SHAKE_DURATION;
                printf("%s used %s! It dealt %d damage.\n", player_pokemon->name, selected_move_name, player_damage);

                // Apply DoT if the rival has a status
                if (rival_pokemon->status != NULL)
                {
                    if (strcmp(rival_pokemon->status, "burned") == 0 || strcmp(rival_pokemon->status, "poisoned") == 0)
                    {
                        int dot_damage = (int)(rival_pokemon->base_stats.hp * 0.0625); // 1/16th of max HP
                        rival_team->hp -= dot_damage;
                        if (rival_team->hp < 0) rival_team->hp = 0;
                        rival_current_hp = rival_team->hp;
                        printf("%s is hurt by %s! It took %d damage.\n", rival_pokemon->name, rival_pokemon->status, dot_damage);
                    }
                }

                // Check if the rival Pokémon fainted
                if (rival_team->hp == 0)
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
                        printf("You win!\n");
                        break;
                    }
                }

                // Switch to rival's turn
                player_turn = 0;
            }
            else
            {
                // Rival attacks
                const char *random_move_name = GetRandomMove(rival_pokemon);
                int rival_damage = CalculateDamage(rival_pokemon, player_pokemon, random_move_name);
                
                // Apply damage to the player
                player_team->hp -= rival_damage;
                if (player_team->hp < 0) player_team->hp = 0;
                player_current_hp = player_team->hp;
                player_shake_timer = SHAKE_DURATION;
                printf("%s used %s! It dealt %d damage.\n", rival_pokemon->name, random_move_name, rival_damage);

                // Apply DoT if the player has a status
                if (player_pokemon->status != NULL)
                {
                    if (strcmp(player_pokemon->status, "burned") == 0 || strcmp(player_pokemon->status, "poisoned") == 0)
                    {
                        int dot_damage = (int)(player_pokemon->base_stats.hp * 0.0625); // 1/16th of max HP
                        player_team->hp -= dot_damage;
                        if (player_team->hp < 0) player_team->hp = 0;
                        player_current_hp = player_team->hp;
                        printf("%s is hurt by %s! It took %d damage.\n", player_pokemon->name, player_pokemon->status, dot_damage);
                    }
                }

                // Check if the player Pokémon fainted
                if (player_team->hp == 0)
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
                        printf("You lose!\n");
                        break;
                    }
                }

                // Switch to player's turn
                player_turn = 1;
            }
        }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Critical hit flash effect
    if (critical_hit_flash_timer > 0)
    {
        ClearBackground(YELLOW);
        critical_hit_flash_timer--;
    }

    // Draw the background
    DrawTexture(battle_bg, 0, 0, WHITE);

    // Set the desired scale
    float pokemon_scale = 2.5f; // Adjust this value for the desired size

    // Draw the player Pokémon (bottom left)
    int player_sprite_width = player_texture.width * pokemon_scale;
    int player_sprite_height = player_texture.height * pokemon_scale;
    int player_x = 220 + player_shake_offset - (player_sprite_width / 2);
    int player_y = 400 - (player_sprite_height / 2);
    DrawTextureEx(player_texture, (Vector2){player_x, player_y}, 0.0f, pokemon_scale, WHITE);
    DrawStatusIcon(player_pokemon, player_x, player_y - 50);

    // Draw the rival Pokémon (top right)
    int rival_sprite_width = rival_texture.width * pokemon_scale;
    int rival_sprite_height = rival_texture.height * pokemon_scale;
    int rival_x = 900 + rival_shake_offset - (rival_sprite_width / 2);
    int rival_y = 200 - (rival_sprite_height / 2);
    DrawTextureEx(rival_texture, (Vector2){rival_x, rival_y}, 0.0f, pokemon_scale, WHITE);
    DrawStatusIcon(rival_pokemon, 830, 60);

    // Draw HP bars
    int player_bar_width = (int)CalculateHPBarWidth(player_current_hp, player_max_hp, HP_BAR_WIDTH);
    int rival_bar_width = (int)CalculateHPBarWidth(rival_current_hp, rival_max_hp, HP_BAR_WIDTH);
    DrawRectangle(150, 550, HP_BAR_WIDTH, 20, DARKGRAY); // Background
    DrawRectangle(150, 550, player_bar_width, 20, GetHPBarColor(player_current_hp, player_max_hp)); // Current HP
    DrawRectangle(850, 50, HP_BAR_WIDTH, 20, DARKGRAY); // Background
    DrawRectangle(850, 50, rival_bar_width, 20, GetHPBarColor(rival_current_hp, rival_max_hp)); // Current HP

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
    UnloadSound(critical_hit_sound);
    UnloadTexture(burn_icon);
    UnloadTexture(paralyzed_icon);
    UnloadTexture(frozen_icon);
    UnloadTexture(poisoned_icon);
    UnloadTexture(sleep_icon);
    UnloadTexture(confused_icon);
    CloseAudioDevice();

}