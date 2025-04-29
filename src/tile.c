#include "tile.h"
#include <stdlib.h>

TileDefinition *tile_defs = NULL;
int tile_defs_count = 0;

void init_tile_definitions(void)
{
    static TileDefinition defs_storage[32]; // espaço para até 32 tiles

    // grama basica (1x1)
    defs_storage[0] = (TileDefinition){0, LoadTexture("assets/tiles/grass/grass01.png")};
    
    // arvore (3x4) 
    //top
    defs_storage[1] = (TileDefinition){100, LoadTexture("assets/tiles/tree/big_tree/big_tree_up01.png")};
    defs_storage[2] = (TileDefinition){101, LoadTexture("assets/tiles/tree/big_tree/big_tree_up02.png")};
    defs_storage[3] = (TileDefinition){102, LoadTexture("assets/tiles/tree/big_tree/big_tree_up03.png")};

    //middle
    defs_storage[4] = (TileDefinition){103, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle01.png")};
    defs_storage[5] = (TileDefinition){104, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle02.png")};
    defs_storage[6] = (TileDefinition){105, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle03.png")};

    defs_storage[7] = (TileDefinition){106, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle04.png")};
    defs_storage[8] = (TileDefinition){107, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle05.png")};
    defs_storage[9] = (TileDefinition){108, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle06.png")};

    //bottom
    defs_storage[10] = (TileDefinition){109, LoadTexture("assets/tiles/tree/big_tree/big_tree_bottom01.png")};
    defs_storage[11] = (TileDefinition){110, LoadTexture("assets/tiles/tree/big_tree/big_tree_bottom02.png")};
    defs_storage[12] = (TileDefinition){111, LoadTexture("assets/tiles/tree/big_tree/big_tree_bottom03.png")};

    // arvore (2x3)
    //top
    defs_storage[13] = (TileDefinition){112, LoadTexture("assets/tiles/tree/small_tree/small_tree_up01.png")};
    defs_storage[14] = (TileDefinition){113, LoadTexture("assets/tiles/tree/small_tree/small_tree_up02.png")};

    //middle
    defs_storage[15] = (TileDefinition){115, LoadTexture("assets/tiles/tree/small_tree/small_tree_middle01.png")};
    defs_storage[16] = (TileDefinition){116, LoadTexture("assets/tiles/tree/small_tree/small_tree_middle02.png")};

    //bottom
    defs_storage[17] = (TileDefinition){118, LoadTexture("assets/tiles/tree/small_tree/small_tree_bottom01.png")};
    defs_storage[18] = (TileDefinition){118, LoadTexture("assets/tiles/tree/small_tree/small_tree_bottom02.png")};

    // plantas basicas (1x1)
    //planta
    defs_storage[19] = (TileDefinition){1, LoadTexture("assets/tiles/plant/plant01.png")};
    
    //flor
    defs_storage[20] = (TileDefinition){2, LoadTexture("assets/tiles/plant/flower01.png")}; 

    // cerca
    //top
    defs_storage[21] = (TileDefinition){50, LoadTexture("assets/tiles/fence/fence_top01.png")};
    defs_storage[22] = (TileDefinition){51, LoadTexture("assets/tiles/fence/fence_top02.png")};
    defs_storage[23] = (TileDefinition){52, LoadTexture("assets/tiles/fence/fence_top03.png")};

    //middle
    defs_storage[24] = (TileDefinition){53, LoadTexture("assets/tiles/fence/fence_middle01.png")};
    defs_storage[25] = (TileDefinition){54, LoadTexture("assets/tiles/fence/fence_middle02.png")};

    //bottom
    defs_storage[26] = (TileDefinition){55, LoadTexture("assets/tiles/fence/fence_bottom01.png")};
    defs_storage[27] = (TileDefinition){56, LoadTexture("assets/tiles/fence/fence_bottom02.png")};
    defs_storage[28] = (TileDefinition){57, LoadTexture("assets/tiles/fence/fence_bottom03.png")};

    tile_defs = defs_storage;
    tile_defs_count = 29; // quantidade de tiles carregados
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

