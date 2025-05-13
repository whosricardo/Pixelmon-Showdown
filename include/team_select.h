#ifndef TEAM_SELECT_H
#define TEAM_SELECT_H

#include "raylib.h"
#include "pokemon_loader.h"

typedef struct {
    Texture2D texture;
    char name[64];
    char generation[16];
    int is_legendary;
} PokemonIcon;

// Function prototypes
void ShowTeamSelection();

#endif
