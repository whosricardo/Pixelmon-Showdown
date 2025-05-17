#ifndef BATTLE_SCREEN_H
#define BATTLE_SCREEN_H

#include "team.h"

void InitBattleScreen(TeamNode *team);
TeamNode* GenerateRivalTeam();
int CalculateDamage(PokemonInfo *attacker, PokemonInfo *defender, const char *move_name);
const char* GetRandomMove(PokemonInfo *pokemon);
const char* GetSelectedMove(PokemonInfo *pokemon, int selected_index);
void DrawBattleMenu(PokemonInfo *pokemon);
void LoadMoveData();
void FreeMoveData();
void LoadTypeChart();  
void FreeTypeChart();  
float GetTypeEffectiveness(const char *move_type, PokemonInfo *defender); 

#endif
