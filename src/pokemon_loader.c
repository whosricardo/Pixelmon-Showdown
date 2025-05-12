#include "file_loader.h"
#include "pokemon_loader.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PokemonInfo *load_pokemon_info(const char *path)
{
    // Load JSON file as a string
    char *json_string = load_file_to_string(path);
    if (!json_string) 
    {
        fprintf(stderr, "ERROR: Failed to load file: %s\n", path);
        return NULL;
    }

    // Parse JSON
    cJSON *root = cJSON_Parse(json_string);
    free(json_string);
    if (!root) 
    {
        fprintf(stderr, "ERROR: Failed to parse JSON: %s\n", path);
        return NULL;
    }

    // Allocate memory for the PokemonInfo struct
    PokemonInfo *pointer = malloc(sizeof(PokemonInfo));
    if (!pointer) 
    {
        fprintf(stderr, "ERROR: Failed to allocate memory for PokemonInfo.\n");
        cJSON_Delete(root);
        return NULL;
    }
    memset(pointer, 0, sizeof(PokemonInfo));

    // Basic Info
    pointer->id = cJSON_GetObjectItem(root, "id")->valueint;
    pointer->name = strdup(cJSON_GetObjectItem(root, "name")->valuestring);

    // Types
    cJSON *types = cJSON_GetObjectItem(root, "type");
    int type_count = cJSON_GetArraySize(types);
    for (int i = 0; i < 2; i++) 
    {
        if (i < type_count) 
        {
            cJSON *t = cJSON_GetArrayItem(types, i);
            pointer->type[i] = t ? strdup(t->valuestring) : NULL;
        }
        else
        {
            pointer->type[i] = NULL;
        }
    }

    // Sprites
    cJSON *sprites = cJSON_GetObjectItem(root, "sprites");
    pointer->sprites.front = strdup(cJSON_GetObjectItem(sprites, "front")->valuestring);
    pointer->sprites.back = strdup(cJSON_GetObjectItem(sprites, "back")->valuestring);

    // Base Stats
    cJSON *stats = cJSON_GetObjectItem(root, "base_stats");
    pointer->base_stats.hp = cJSON_GetObjectItem(stats, "hp")->valueint;
    pointer->base_stats.attack = cJSON_GetObjectItem(stats, "attack")->valueint;
    pointer->base_stats.defense = cJSON_GetObjectItem(stats, "defense")->valueint;
    pointer->base_stats.sp_attack = cJSON_GetObjectItem(stats, "sp_attack")->valueint;
    pointer->base_stats.sp_defense = cJSON_GetObjectItem(stats, "sp_defense")->valueint;
    pointer->base_stats.speed = cJSON_GetObjectItem(stats, "speed")->valueint;

    // Abilities
    cJSON *abilities = cJSON_GetObjectItem(root, "abilities");
    int ability_count = cJSON_GetArraySize(abilities);
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        if (i < ability_count) 
        {
            cJSON *ab = cJSON_GetArrayItem(abilities, i);
            pointer->abilities[i] = ab ? strdup(ab->valuestring) : NULL;
        }
        else
        {
            pointer->abilities[i] = NULL;
        }
    }

    // Evolution
    cJSON *evo = cJSON_GetObjectItem(root, "evolves_to");
    if (evo && !cJSON_IsNull(evo)) 
    {
        cJSON *evo_name = cJSON_GetObjectItem(evo, "name");
        pointer->evolves_to = evo_name ? strdup(evo_name->valuestring) : NULL;
    }
    else
    {
        pointer->evolves_to = NULL;
    }

    // Learnset (Now Dynamic)
    cJSON *learnset = cJSON_GetObjectItem(root, "learnset");
    int move_count = cJSON_GetArraySize(learnset);
    pointer->learnset_size = move_count;
    pointer->learnset = malloc(move_count * sizeof(LearnMove));

    for (int i = 0; i < move_count; i++) 
    {
        cJSON *move = cJSON_GetArrayItem(learnset, i);
        pointer->learnset[i].level = cJSON_GetObjectItem(move, "level")->valueint;
        pointer->learnset[i].move = strdup(cJSON_GetObjectItem(move, "move")->valuestring);
    }

    cJSON_Delete(root);
    return pointer;
}

void free_pokemon(PokemonInfo *pointer) 
{
    if (!pointer) return;

    free(pointer->name);

    for (int i = 0; i < 2; i++) 
        free(pointer->type[i]);

    free(pointer->sprites.front);
    free(pointer->sprites.back);

    for (int i = 0; i < MAX_ABILITIES; i++) 
        free(pointer->abilities[i]);

    free(pointer->evolves_to);

    for (int i = 0; i < pointer->learnset_size; i++) 
        free(pointer->learnset[i].move);

    free(pointer->learnset);
    free(pointer);
}
