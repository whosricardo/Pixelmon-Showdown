#ifndef MAPA_H
#define MAPA_H

#define MAP_WIDTH  50
#define MAP_HEIGHT 38

extern int mapa[MAP_HEIGHT][MAP_WIDTH];

void colocar_arvore(int x, int y);
// Função que inicializa o mapa
void init_mapa(void);

#endif
