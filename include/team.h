#ifndef TEAM_H
#define TEAM_H

#include "pokemon_loader.h"

// Linked list node for a player's team
typedef struct TeamNode
{
    int pokemon_index;
    PokemonInfo *pokemon;
    struct TeamNode *next;
} TeamNode;

// Function prototypes
TeamNode* create_team_node(int index, PokemonInfo *pokemon);
void add_to_team(TeamNode **head, int index, PokemonInfo *pokemon);
void print_team(TeamNode *head);
void free_team(TeamNode *head);
int team_size(TeamNode *head);
int is_pokemon_in_team(TeamNode *head, int index);

#endif
