#include "player_loader.h"
#include "file_loader.h"
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static DirectionalAnimation load_directional_animation(cJSON *animation_obj);
static SimpleAnimation load_simple_animation(cJSON *animation_array);

PlayerAnimations *load_player_animations(const char *path)
{
    char *json_string = load_file_to_string(path); // Corrigido para char*
    if (!json_string) return NULL;

    cJSON *root = cJSON_Parse(json_string); // cria o objeto JSON
    free(json_string); // Liberar a string lida depois de parsear
    if (!root) return NULL;

    PlayerAnimations *animations = malloc(sizeof(PlayerAnimations));
    if (!animations) {
        cJSON_Delete(root);
        return NULL;
    }

    // Carrega cada animação usando funções auxiliares
    animations->walk = load_directional_animation(cJSON_GetObjectItem(root, "walk_animation"));
    animations->run = load_directional_animation(cJSON_GetObjectItem(root, "run_animation"));
    animations->bike = load_directional_animation(cJSON_GetObjectItem(root, "bike_animation"));
    animations->bike_acro = load_directional_animation(cJSON_GetObjectItem(root, "bike_acro_animation"));
    animations->battle = load_simple_animation(cJSON_GetObjectItem(root, "battle_animation"));
    
    // Carrega frame_duration
    cJSON *frame_duration_item = cJSON_GetObjectItem(root, "frame_duration");
    animations->frame_duration = frame_duration_item ? (float)frame_duration_item->valuedouble : 0.15f;

    cJSON_Delete(root);
    return animations;
}

void free_player_animations(PlayerAnimations *animations)
{
    if (!animations) return;

    // Free walk
    for (int i = 0; i < animations->walk.frame_count; i++) {
        free(animations->walk.down[i]);
        free(animations->walk.up[i]);
        free(animations->walk.left[i]);
        free(animations->walk.right[i]);
    }
    free(animations->walk.down);
    free(animations->walk.up);
    free(animations->walk.left);
    free(animations->walk.right);

    // Free run
    for (int i = 0; i < animations->run.frame_count; i++) {
        free(animations->run.down[i]);
        free(animations->run.up[i]);
        free(animations->run.left[i]);
        free(animations->run.right[i]);
    }
    free(animations->run.down);
    free(animations->run.up);
    free(animations->run.left);
    free(animations->run.right);

    // Free bike
    for (int i = 0; i < animations->bike.frame_count; i++) {
        free(animations->bike.down[i]);
        free(animations->bike.up[i]);
        free(animations->bike.left[i]);
        free(animations->bike.right[i]);
    }
    free(animations->bike.down);
    free(animations->bike.up);
    free(animations->bike.left);
    free(animations->bike.right);

    // Free bike_acro
    for (int i = 0; i < animations->bike_acro.frame_count; i++) {
        free(animations->bike_acro.down[i]);
        free(animations->bike_acro.up[i]);
        free(animations->bike_acro.left[i]);
        free(animations->bike_acro.right[i]);
    }
    free(animations->bike_acro.down);
    free(animations->bike_acro.up);
    free(animations->bike_acro.left);
    free(animations->bike_acro.right);

    // Free battle
    for (int i = 0; i < animations->battle.frame_count; i++) {
        free(animations->battle.frames[i]);
    }
    free(animations->battle.frames);

    // Free main struct
    free(animations);
}

// Função auxiliar: carrega uma DirectionalAnimation (walk, run, bike, bike_acro)
static DirectionalAnimation load_directional_animation(cJSON *animation_obj)
{
    DirectionalAnimation anim = {0};

    if (!animation_obj) return anim;

    const char *directions[] = {"down", "up", "right", "left"};
    char ***target_arrays[] = {&anim.down, &anim.up, &anim.right, &anim.left};

    // Ver quantos frames tem (assumindo que todos os lados têm o mesmo número de frames)
    cJSON *down_array = cJSON_GetObjectItem(animation_obj, "down");
    anim.frame_count = cJSON_GetArraySize(down_array);

    for (int d = 0; d < 4; d++) {
        cJSON *direction_array = cJSON_GetObjectItem(animation_obj, directions[d]);
        if (!direction_array) continue;

        *target_arrays[d] = malloc(sizeof(char*) * anim.frame_count);

        for (int i = 0; i < anim.frame_count; i++) {
            const char *filename = cJSON_GetArrayItem(direction_array, i)->valuestring;
            (*target_arrays[d])[i] = strdup(filename); // Copia a string
        }
    }

    return anim;
}

// Função auxiliar: carrega uma SimpleAnimation (battle)
static SimpleAnimation load_simple_animation(cJSON *animation_array)
{
    SimpleAnimation anim = {0};

    if (!animation_array) return anim;

    anim.frame_count = cJSON_GetArraySize(animation_array);
    anim.frames = malloc(sizeof(char*) * anim.frame_count);

    for (int i = 0; i < anim.frame_count; i++) {
        const char *filename = cJSON_GetArrayItem(animation_array, i)->valuestring;
        anim.frames[i] = strdup(filename); // Copia a string
    }

    return anim;
}
