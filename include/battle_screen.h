#ifndef BATTLE_SCREEN_H
#define BATTLE_SCREEN_H

#include "team.h"

void InitBattleScreen(TeamNode *team);
void DrawBattleScreen();
void UpdateBattleScreen();
void UnloadBattleScreen();
TeamNode* GenerateRivalTeam();

#endif
