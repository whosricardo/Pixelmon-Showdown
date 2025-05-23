#include "pokemon_loader.h"
#include "file_loader.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PokemonInfo *load_pokemon_info(const char *path)
{
    char *json_string = load_file_to_string(path);
    if (!json_string) 
    {
        fprintf(stderr, "ERROR: Failed to load file: %s\n", path);
        return NULL;
    }

    cJSON *root = cJSON_Parse(json_string);
    free(json_string);
    if (!root) 
    {
        fprintf(stderr, "ERROR: Failed to parse JSON: %s\n", path);
        return NULL;
    }

    PokemonInfo *pointer = malloc(sizeof(PokemonInfo));
    if (!pointer) 
    {
        fprintf(stderr, "ERROR: Failed to allocate memory for PokemonInfo.\n");
        cJSON_Delete(root);
        return NULL;
    }
    memset(pointer, 0, sizeof(PokemonInfo));

    // Basic Info
    cJSON *id_item = cJSON_GetObjectItem(root, "id");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *legendary_item = cJSON_GetObjectItem(root, "is_legendary");

    if (!cJSON_IsNumber(id_item) || !cJSON_IsString(name_item))
    {
        fprintf(stderr, "ERROR: Missing or invalid 'id' or 'name' in %s\n", path);
        free(pointer);
        cJSON_Delete(root);
        return NULL;
    }

    pointer->id = id_item->valueint;
    pointer->name = strdup(name_item->valuestring);
    pointer->is_legendary = cJSON_IsTrue(legendary_item);

    // Types
    cJSON *types = cJSON_GetObjectItem(root, "type");
    if (cJSON_IsArray(types))
    {
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
    }

    // Sprites (front, back, icon)
    cJSON *sprites = cJSON_GetObjectItem(root, "sprites");
    if (cJSON_IsObject(sprites))
    {
        cJSON *front_item = cJSON_GetObjectItem(sprites, "front");
        cJSON *back_item = cJSON_GetObjectItem(sprites, "back");
        cJSON *icon_item = cJSON_GetObjectItem(sprites, "icon");

        // Get the directory path from the main JSON file path
        char base_path[256];
        strcpy(base_path, path);
        char *last_slash = strrchr(base_path, '/');
        if (last_slash) *(last_slash + 1) = '\0'; // Keep the directory part

        // Construct full paths for each sprite
        if (front_item && cJSON_IsString(front_item))
        {
            char full_front_path[512];
            snprintf(full_front_path, sizeof(full_front_path), "%s%s", base_path, front_item->valuestring);
            pointer->sprites.front = strdup(full_front_path);
        }

        if (back_item && cJSON_IsString(back_item))
        {
            char full_back_path[512];
            snprintf(full_back_path, sizeof(full_back_path), "%s%s", base_path, back_item->valuestring);
            pointer->sprites.back = strdup(full_back_path);
        }

        if (icon_item && cJSON_IsString(icon_item))
        {
            char full_icon_path[512];
            snprintf(full_icon_path, sizeof(full_icon_path), "%s%s", base_path, icon_item->valuestring);
            pointer->sprites.icon = strdup(full_icon_path);
        }
    }

        // Load the base stats
    cJSON *base_stats = cJSON_GetObjectItem(root, "base_stats");
    if (cJSON_IsObject(base_stats))
    {
        cJSON *hp_item = cJSON_GetObjectItem(base_stats, "hp");
        cJSON *attack_item = cJSON_GetObjectItem(base_stats, "attack");
        cJSON *defense_item = cJSON_GetObjectItem(base_stats, "defense");
        cJSON *sp_attack_item = cJSON_GetObjectItem(base_stats, "sp_attack");
        cJSON *sp_defense_item = cJSON_GetObjectItem(base_stats, "sp_defense");
        cJSON *speed_item = cJSON_GetObjectItem(base_stats, "speed");

        if (cJSON_IsNumber(hp_item)) pointer->base_stats.hp = hp_item->valueint;
        if (cJSON_IsNumber(attack_item)) pointer->base_stats.attack = attack_item->valueint;
        if (cJSON_IsNumber(defense_item)) pointer->base_stats.defense = defense_item->valueint;
        if (cJSON_IsNumber(sp_attack_item)) pointer->base_stats.sp_attack = sp_attack_item->valueint;
        if (cJSON_IsNumber(sp_defense_item)) pointer->base_stats.sp_defense = sp_defense_item->valueint;
        if (cJSON_IsNumber(speed_item)) pointer->base_stats.speed = speed_item->valueint;
    }

    // Learnset
    cJSON *learnset = cJSON_GetObjectItem(root, "learnset");
    if (cJSON_IsArray(learnset))
    {
        int move_count = cJSON_GetArraySize(learnset);
        pointer->learnset = malloc(move_count * sizeof(LearnMove));
        pointer->learnset_size = move_count;

        for (int i = 0; i < pointer->learnset_size; i++)
        {
            cJSON *move_item = cJSON_GetArrayItem(learnset, i);
            if (cJSON_IsString(move_item))
            {
                pointer->learnset[i].move = strdup(move_item->valuestring);
            }
        }
    }
    else
    {
        pointer->learnset = NULL;
        pointer->learnset_size = 0;
    }

    cJSON_Delete(root);
    return pointer;
}

void free_pokemon(PokemonInfo *pointer) 
{
    if (!pointer) return;

    free(pointer->name);
    free(pointer->sprites.front);
    free(pointer->sprites.back);
    free(pointer->sprites.icon);
    free(pointer->evolves_to);
    free(pointer->pre_evolution);

    for (int i = 0; i < 2; i++) 
        free(pointer->type[i]);

    for (int i = 0; i < MAX_ABILITIES; i++) 
        free(pointer->abilities[i]);

    if (pointer->learnset)
    {
        for (int i = 0; i < pointer->learnset_size; i++)
            free(pointer->learnset[i].move);
        free(pointer->learnset);
    }
    
    if (pointer->status)
    free(pointer->status);

    free(pointer);
}
