#include "team.h"
#include <stdio.h>
#include <stdlib.h>

// Create a new node
TeamNode* create_team_node(int index, PokemonInfo *pokemon)
{
    TeamNode *node = malloc(sizeof(TeamNode));
    if (!node)
    {
        fprintf(stderr, "ERROR: Failed to allocate memory for team node.\n");
        return NULL;
    }

    node->pokemon_index = index;
    node->pokemon = pokemon;
    node->next = NULL;
    return node;
}

// Add a node to the end of the list
void add_to_team(TeamNode **head, int index, PokemonInfo *pokemon)
{
    TeamNode *new_node = create_team_node(index, pokemon);
    if (!new_node) return;

    if (*head == NULL)
    {
        *head = new_node;
        return;
    }

    TeamNode *current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_node;
}

// Print the team for debugging
void print_team(TeamNode *head)
{
    TeamNode *current = head;
    printf("Current Team:\n");
    while (current != NULL)
    {
        printf(" - %s\n", current->pokemon->name);
        current = current->next;
    }
}

// Free the entire team list
void free_team(TeamNode *head)
{
    TeamNode *current = head;
    while (current != NULL)
    {
        TeamNode *next = current->next;
        free(current);
        current = next;
    }
}

// Check if a Pokémon is already in the team
int is_pokemon_in_team(TeamNode *head, int index)
{
    TeamNode *current = head;
    while (current != NULL)
    {
        if (current->pokemon_index == index) return 1;
        current = current->next;
    }
    return 0;
}

// Count the number of Pokémon in the team
int team_size(TeamNode *head)
{
    int count = 0;
    TeamNode *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}
