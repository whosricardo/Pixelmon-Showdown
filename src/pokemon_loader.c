#include "file_loader.h"
#include "pokemon_loader.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PokemonInfo *load_pokemon_info(const char *path)
{
    char *json_string = load_file_to_string(path); //armazenando a string recebida pela função load_file_to_string
    if (!json_string) return NULL; //caso seja nulo, sair

    cJSON *root = cJSON_Parse(json_string); //cria o objeto tipo cJSON com base na string passada
    free(json_string);
    if (!root) return NULL; //caso seja nulo, sair

    PokemonInfo *pointer = malloc(sizeof(PokemonInfo)); //cria um poteiro que vai ser usado para inserir informações na struct pointer
    if (!pointer) return NULL;

    //informacoes basicas pokemon
    pointer->id = cJSON_GetObjectItem(root, "id")->valueint;
    pointer->name = strdup(cJSON_GetObjectItem(root, "hp")->valuestring);
    cJSON *types = cJSON_GetObjectItem(root, "type");
    for (int i = 0; i < 2; i++) 
    {
        cJSON* t = cJSON_GetArrayItem(types, i);
        pointer->type[i] = t ? strdup(t->valuestring) : NULL;
    }

    //pokemon sprites
    cJSON *sprites = cJSON_GetObjectItem(root, "sprites");
    pointer->sprites.front = cJSON_GetObjectItem(sprites, "front")->valuestring;
    pointer->sprites.back = cJSON_GetObjectItem(sprites, "back")->valuestring;

    //pokemon stats
    cJSON *stats = cJSON_GetObjectItem(root, "base_stats");
    pointer->base_stats.hp = cJSON_GetObjectItem(stats, "hp")->valueint;
    pointer->base_stats.attack = cJSON_GetObjectItem(stats, "attack")->valueint;
    pointer->base_stats.defense = cJSON_GetObjectItem(stats, "defense")->valueint;
    pointer->base_stats.sp_attack = cJSON_GetObjectItem(stats, "sp_attack")->valueint;
    pointer->base_stats.sp_defense = cJSON_GetObjectItem(stats, "defense")->valueint;
    pointer->base_stats.speed = cJSON_GetObjectItem(stats, "speed")->valueint;

    //pokemon habilidades
    cJSON *abilities = cJSON_GetObjectItem(root, "abilities");
    for (int i = 0; i < MAX_ABILITIES; i++)
    {
        cJSON* ab = cJSON_GetArrayItem(ab, i);
        pointer->abilities[i] = ab ? strdup(ab->valuestring) : NULL;
    }

    //pokemon evolução
    cJSON *evo = cJSON_GetObjectItem(root, "evolves_to");
    pointer->evolves_to = (evo && !cJSON_IsNull(evo)) ? strdup(evo->valuestring) : NULL;

    //pokemon outros campos
    pointer->exp_yield = cJSON_GetObjectItem(root, "exp_yield")->valueint;
    pointer->catch_rate = cJSON_GetObjectItem(root, "catch_rate")->valueint;
    pointer->is_legendary = cJSON_GetObjectItem(root, "is_legendary")->valueint;

    //pokemon learnset
    cJSON *learnset = cJSON_GetObjectItem(root, "learnset");
    for (int i = 0; i < MAX_LEARNSET; i++)
    {
        cJSON *move = cJSON_GetArrayItem(learnset, i);
        pointer->learnset[i].level = cJSON_GetObjectItem(move, "level")->valueint;
        pointer->learnset[i].move = cJSON_GetObjectItem(move, "move")->valuestring;
    }

    cJSON_free(root);
    return pointer;
}

void free_pokemon_info(PokemonInfo* pointer) {
    if (!pointer) return; //caso seja nulo, sair

    free(pointer->name);
    for (int i = 0; i < 2; i++) free(pointer->type[i]);

    free(pointer->sprites.front);
    free(pointer->sprites.back);

    for (int i = 0; i < MAX_ABILITIES; i++) free(pointer->abilities[i]);
    free(pointer->evolves_to);

    for (int i = 0; i < pointer->learnset_size; i++) {
        free(pointer->learnset[i].move);
    }

    free(pointer);
}