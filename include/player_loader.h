#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H

#include "raylib.h"

#define MAX_FRAMES 10

typedef struct {
    Texture2D frames[MAX_FRAMES];
    int frameCount;
    float frameDuration;
} Animation;

typedef struct {
    Animation battle;
    Texture2D rival_show;
} Player;

void load_player_battle_animation(Player *player, const char *jsonPath);
void free_player(Player *player);

#endif
