#ifndef TILE_H
#define TILE_H

#include "raylib.h"

#define TILE_SIZE 16

typedef struct {
    int id;
    Texture2D texture;
} TileDefinition;

extern TileDefinition *tile_defs;
extern int tile_defs_count;

void init_tile_definitions(void);
TileDefinition *get_tile_by_id(int id);
void unload_tile_definitions(void);

#endif
