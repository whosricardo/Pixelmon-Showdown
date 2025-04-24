#include <stdio.h>
#include "../include/pokemon_loader.h"

int main() {
    const char *json_path = "./assets/sprites/pokemons/gen3/blaziken/blaziken.json";

    PokemonInfo *pokemon = load_pokemon_info(json_path);
    if (pokemon == NULL) {
        printf("Erro ao carregar Pokémon.\n");
        return 1;
    }

    printf("ID: %d\n", pokemon->id);
    printf("Nome: %s\n", pokemon->name);
    printf("Tipos: %s, %s\n", pokemon->type[0], pokemon->type[1]);
    printf("HP: %d\n", pokemon->base_stats.hp);
    printf("Ataque: %d\n", pokemon->base_stats.attack);
    printf("Defesa: %d\n", pokemon->base_stats.defense);
    printf("Habilidade 1: %s\n", pokemon->abilities[0]);
    printf("EXP: %d\n", pokemon->exp_yield);
    printf("Legendário? %s\n", pokemon->is_legendary ? "Sim" : "Não");

    printf("Learnset:\n");
    for (int i = 0; i < pokemon->learnset_size; i++) {
        printf("  Nível %d: %s\n", pokemon->learnset[i].level, pokemon->learnset[i].move);
    }

    free_pokemon(pokemon);
    return 0;
}
