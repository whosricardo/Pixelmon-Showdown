#include "rival_loader.h"
#include "file_loader.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_rival_sprite(Rival *rival, const char *jsonPath) {
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

    // Load the rival sprite
    cJSON *rivalItem = cJSON_GetObjectItem(root, "rival_show");
    if (cJSON_IsString(rivalItem) && rivalItem->valuestring) {
        rival->battle = LoadTexture(rivalItem->valuestring);

        if (rival->battle.id == 0) {
            fprintf(stderr, "ERROR: Failed to load rival texture from %s\n", rivalItem->valuestring);
        } else {
            printf("Loaded rival texture from %s\n", rivalItem->valuestring);
        }
    } else {
        fprintf(stderr, "ERROR: 'rival_show' is not a valid string in %s\n", jsonPath);
    }

    cJSON_Delete(root);
}

void free_rival(Rival *rival) {
    UnloadTexture(rival->battle);
}
