#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H

#include "raylib.h"

typedef struct {
    Texture2D battle;
    Texture2D rival_show;
} Player;

void load_player_battle_animation(Player *player, const char *jsonPath);
void free_player(Player *player);

#endif
