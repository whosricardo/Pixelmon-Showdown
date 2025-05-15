#ifndef RIVAL_LOADER_H
#define RIVAL_LOADER_H

#include "raylib.h"

typedef struct {
    Texture2D battle;
} Rival;

void load_rival_sprite(Rival *rival, const char *jsonPath);
void free_rival(Rival *rival);

#endif
