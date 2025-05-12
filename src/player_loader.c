#include "player_loader.h"
#include "file_loader.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_player_battle_animation(Player *player, const char *jsonPath) {
    char *jsonData = load_file_to_string(jsonPath);
    if (!jsonData) {
        fprintf(stderr, "ERROR: Failed to load file: %s\n", jsonPath);
        return;
    }

    cJSON *root = cJSON_Parse(jsonData);
    free(jsonData);

    if (!root) {
        fprintf(stderr, "ERROR: Failed to parse JSON: %s\n", jsonPath);
        return;
    }

    // Load battle animations
    cJSON *battleAnim = cJSON_GetObjectItem(root, "battle_animation");
    if (!cJSON_IsArray(battleAnim)) {
        fprintf(stderr, "ERROR: 'battle_animation' is not a valid array in %s\n", jsonPath);
        cJSON_Delete(root);
        return;
    }

    player->battle.frameCount = cJSON_GetArraySize(battleAnim);
    if (player->battle.frameCount > MAX_FRAMES) {
        fprintf(stderr, "ERROR: Too many frames in %s. Maximum allowed is %d.\n", jsonPath, MAX_FRAMES);
        cJSON_Delete(root);
        return;
    }

    cJSON *frameDurationItem = cJSON_GetObjectItem(root, "frame_duration");
    if (!cJSON_IsNumber(frameDurationItem)) {
        fprintf(stderr, "ERROR: 'frame_duration' is not a valid number in %s\n", jsonPath);
        cJSON_Delete(root);
        return;
    }
    player->battle.frameDuration = frameDurationItem->valuedouble;

    for (int i = 0; i < player->battle.frameCount; i++) {
        const char *filename = cJSON_GetArrayItem(battleAnim, i)->valuestring;
        if (!filename) {
            fprintf(stderr, "ERROR: Invalid frame filename at index %d in %s\n", i, jsonPath);
            continue;
        }

        char fullpath[256];
        snprintf(fullpath, sizeof(fullpath), "assets/sprites/player/%s", filename);
        player->battle.frames[i] = LoadTexture(fullpath);
    }

    // Load rival sprite
    const char *rivalFile = cJSON_GetObjectItem(root, "rival_show")->valuestring;
    if (rivalFile) {
        char rivalFullPath[256];
        snprintf(rivalFullPath, sizeof(rivalFullPath), "assets/sprites/player/%s", rivalFile);
        player->rival_show = LoadTexture(rivalFullPath);
    }

    cJSON_Delete(root);
}

void free_player(Player *player) {
    for (int i = 0; i < player->battle.frameCount; i++) {
        UnloadTexture(player->battle.frames[i]);
    }
    UnloadTexture(player->rival_show);
}
