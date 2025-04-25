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
    // Preenche todo o mapa com grama
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            mapa[y][x] = 0; // 0 = grama
        }
    }

    // Cerca com árvores nas bordas (esquerda e direita)
    for (int y = 0; y < MAP_HEIGHT - 4; y += 5) {
        colocar_arvore(0, y);
        colocar_arvore(MAP_WIDTH - 3, y);
    }

    // Algumas árvores espalhadas
    colocar_arvore(10, 5);
    colocar_arvore(20, 12);
    colocar_arvore(35, 8);
    colocar_arvore(15, 20);
    colocar_arvore(5, 25);
    colocar_arvore(25, 30);
    colocar_arvore(40, 18);
}