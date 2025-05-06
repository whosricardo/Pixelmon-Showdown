#include "map.h"

int mapa[MAP_HEIGHT][MAP_WIDTH];

void colocar_arvore(int x, int y) {
    int ids[4][3] = {
        {100, 101, 102},
        {103, 104, 105},
        {106, 107, 108},
        {109, 110, 111}
    };

    for (int linha = 0; linha < 4; linha++) {
        for (int coluna = 0; coluna < 3; coluna++) {
            if (y + linha < MAP_HEIGHT && x + coluna < MAP_WIDTH) {
                mapa[y + linha][x + coluna] = ids[linha][coluna];
            }
        }
    }
}

void init_mapa() {
    // Preenche com grama padrão (id = 0)
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            mapa[y][x] = 0;
        }
    }

    // Grama alta (usaremos 200 = planta, só para representar)
    for (int y = 3; y <= 4; y++) for (int x = 5; x <= 10; x++) mapa[y][x] = 200;
    for (int y = 7; y <= 8; y++) for (int x = 13; x <= 18; x++) mapa[y][x] = 200;
    for (int y = 13; y <= 14; y++) for (int x = 8; x <= 14; x++) mapa[y][x] = 200;
    for (int y = 20; y <= 21; y++) for (int x = 15; x <= 20; x++) mapa[y][x] = 200;

    // Caminho de terra (fill = 80)
    for (int x = 7; x <= 16; x++) mapa[1][x] = 80;
    for (int x = 7; x <= 14; x++) mapa[5][x] = 80;
    for (int x = 10; x <= 18; x++) mapa[10][x] = 80;
    for (int x = 8; x <= 12; x++) mapa[15][x] = 80;
    for (int x = 5; x <= 22; x++) mapa[22][x] = 80;

    // Cercas (topo inferior da rota)
    for (int x = 3; x <= 21; x++) mapa[23][x] = 50;

    // Árvores laterais (esquerda/direita)
    for (int y = 0; y < MAP_HEIGHT; y += 5) colocar_arvore(0, y);
    for (int y = 0; y < MAP_HEIGHT; y += 5) colocar_arvore(MAP_WIDTH - 3, y);

    // Árvores internas (para dar estilo à Route 1)
    colocar_arvore(7, 2);
    colocar_arvore(12, 6);
    colocar_arvore(18, 10);
    colocar_arvore(22, 15);
    colocar_arvore(28, 20);
}
