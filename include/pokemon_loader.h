#ifndef POKEMON_LOADER_H
#define POKEMON_LOADER_H
#define MAX_ABILITIES 2

typedef struct 
{
    char *front;
    char *back;
} SpritePaths;

typedef struct 
{
    int level;
    char *move;
} LearnMove;

typedef struct 
{
    int hp;
    int attack;
    int defense;
    int sp_attack;
    int sp_defense;
    int speed;
} BaseStats;

typedef struct
{
    int id;
    char *name;
    char *type[2];

    SpritePaths sprites;
    BaseStats base_stats;

    char *abilities[MAX_ABILITIES];
    char *evolves_to;

    int is_legendary; // 0 = false, 1 = true

    LearnMove *learnset;
    int learnset_size;
} PokemonInfo;

PokemonInfo* load_pokemon_info(const char *path);
void free_pokemon(PokemonInfo *info);

#endif
