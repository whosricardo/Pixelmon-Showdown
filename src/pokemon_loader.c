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
    pointer->is_legendary = cJSON_IsTrue(legendary_item);  // FIXED: Correct legendary parsing

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

        pointer->sprites.front = front_item && cJSON_IsString(front_item) ? strdup(front_item->valuestring) : NULL;
        pointer->sprites.back = back_item && cJSON_IsString(back_item) ? strdup(back_item->valuestring) : NULL;
        pointer->sprites.icon = icon_item && cJSON_IsString(icon_item) ? strdup(icon_item->valuestring) : NULL;
    }

    // Evolution
    cJSON *evo = cJSON_GetObjectItem(root, "evolves_to");
    if (evo && !cJSON_IsNull(evo)) 
    {
        cJSON *evo_name = cJSON_GetObjectItem(evo, "name");
        pointer->evolves_to = evo_name && cJSON_IsString(evo_name) ? strdup(evo_name->valuestring) : NULL;
    }

    // Pre-Evolution
    cJSON *pre_evo = cJSON_GetObjectItem(root, "pre_evolution");
    if (pre_evo && !cJSON_IsNull(pre_evo))
    {
        cJSON *pre_evo_name = cJSON_GetObjectItem(pre_evo, "name");
        pointer->pre_evolution = pre_evo_name && cJSON_IsString(pre_evo_name) ? strdup(pre_evo_name->valuestring) : NULL;
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

    free(pointer);
}
