#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H

#include "raylib.h"

typedef struct {
    Texture2D battle;
} Player;

void load_player_sprite(Player *player, const char *jsonPath);
void free_player(Player *player);

#endif
