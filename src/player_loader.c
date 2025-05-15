#include "player_loader.h"
#include "file_loader.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_player_sprite(Player *player, const char *jsonPath) {
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

    // Load the player sprite
    cJSON *playerItem = cJSON_GetObjectItem(root, "player_show");
    if (cJSON_IsString(playerItem) && playerItem->valuestring) {
        player->battle = LoadTexture(playerItem->valuestring);

        if (player->battle.id == 0) {
            fprintf(stderr, "ERROR: Failed to load player texture from %s\n", playerItem->valuestring);
        } else {
            printf("Loaded player texture from %s\n", playerItem->valuestring);
        }
    } else {
        fprintf(stderr, "ERROR: 'player_show' is not a valid string in %s\n", jsonPath);
    }

    cJSON_Delete(root);
}

void free_player(Player *player) {
    UnloadTexture(player->battle);
}
