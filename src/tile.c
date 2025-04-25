#include "tile.h"
#include <stdlib.h>

TileDefinition *tile_defs = NULL;
int tile_defs_count = 0;

void init_tile_definitions(void)
{
    static TileDefinition defs_storage[32]; // espaço para até 32 tiles

    defs_storage[0] = (TileDefinition){0, LoadTexture("assets/tiles/grama/grama01.png")};

    // Árvore (3x4) - cada pedaço é um arquivo diferente
    defs_storage[1] = (TileDefinition){100, LoadTexture("assets/tiles/arvore/arvore01.png")};
    defs_storage[2] = (TileDefinition){101, LoadTexture("assets/tiles/arvore/arvore02.png")};
    defs_storage[3] = (TileDefinition){102, LoadTexture("assets/tiles/arvore/arvore03.png")};

    defs_storage[4] = (TileDefinition){103, LoadTexture("assets/tiles/arvore/arvore04.png")};
    defs_storage[5] = (TileDefinition){104, LoadTexture("assets/tiles/arvore/arvore05.png")};
    defs_storage[6] = (TileDefinition){105, LoadTexture("assets/tiles/arvore/arvore06.png")};

    defs_storage[7] = (TileDefinition){106, LoadTexture("assets/tiles/arvore/arvore07.png")};
    defs_storage[8] = (TileDefinition){107, LoadTexture("assets/tiles/arvore/arvore08.png")};
    defs_storage[9] = (TileDefinition){108, LoadTexture("assets/tiles/arvore/arvore09.png")};

    defs_storage[10] = (TileDefinition){109, LoadTexture("assets/tiles/arvore/arvore10.png")};
    defs_storage[11] = (TileDefinition){110, LoadTexture("assets/tiles/arvore/arvore11.png")};
    defs_storage[12] = (TileDefinition){111, LoadTexture("assets/tiles/arvore/arvore12.png")};

    tile_defs = defs_storage;
    tile_defs_count = 13; // Quantidade de tiles carregados
}

TileDefinition *get_tile_by_id(int id)
{
    for (int i = 0; i < tile_defs_count; i++) {
        if (tile_defs[i].id == id) {
            return &tile_defs[i];
        }
    }
    return NULL;
}

void unload_tile_definitions(void)
{
    for (int i = 0; i < tile_defs_count; i++) {
        UnloadTexture(tile_defs[i].texture);
    }
}

