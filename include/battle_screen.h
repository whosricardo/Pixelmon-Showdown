#ifndef BATTLE_SCREEN_H
#define BATTLE_SCREEN_H

#include "team.h"

void InitBattleScreen(TeamNode *team);
void DrawBattleScreen();
void UpdateBattleScreen();
void UnloadBattleScreen();
TeamNode* GenerateRivalTeam();
int CalculateDamage(PokemonInfo *attacker, PokemonInfo *defender);
const char* GetRandomMove(PokemonInfo *pokemon);

#endif
