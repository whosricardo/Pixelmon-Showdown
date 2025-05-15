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

    // Load battle sprite
    cJSON *battleAnimItem = cJSON_GetObjectItem(root, "battle_animation");
    if (cJSON_IsString(battleAnimItem) && battleAnimItem->valuestring) {
        char battlePath[256];
        snprintf(battlePath, sizeof(battlePath), "assets/sprites/player/%s", battleAnimItem->valuestring);
        player->battle = LoadTexture(battlePath);
    } else {
        fprintf(stderr, "ERROR: 'battle_animation' is not a valid string in %s\n", jsonPath);
    }

    // Load rival sprite
    cJSON *rivalItem = cJSON_GetObjectItem(root, "rival_show");
    if (cJSON_IsString(rivalItem) && rivalItem->valuestring) {
        char rivalPath[256];
        snprintf(rivalPath, sizeof(rivalPath), "assets/sprites/player/%s", rivalItem->valuestring);
        player->rival_show = LoadTexture(rivalPath);
    } else {
        fprintf(stderr, "ERROR: 'rival_show' is not a valid string in %s\n", jsonPath);
    }

    cJSON_Delete(root);
}

void free_player(Player *player) {
    UnloadTexture(player->battle);
    UnloadTexture(player->rival_show);
}
