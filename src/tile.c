#include "tile.h"
#include <stdlib.h>

TileDefinition *tile_defs = NULL;
int tile_defs_count = 0;

void init_tile_definitions(void)
{
    static TileDefinition defs_storage[51]; // espaço para até 51 tiles

    // grama basica (1x1)
    defs_storage[0] = (TileDefinition){0, LoadTexture("assets/tiles/grass/grass01.png"), 0};
    
    // arvore (4x3) 
    //top
    defs_storage[1] = (TileDefinition){100, LoadTexture("assets/tiles/tree/big_tree/big_tree_up01.png"), 1};
    defs_storage[2] = (TileDefinition){101, LoadTexture("assets/tiles/tree/big_tree/big_tree_up02.png"), 1};
    defs_storage[3] = (TileDefinition){102, LoadTexture("assets/tiles/tree/big_tree/big_tree_up03.png"), 1};

    //middle
    defs_storage[4] = (TileDefinition){103, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle01.png"), 1};
    defs_storage[5] = (TileDefinition){104, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle02.png"), 1};
    defs_storage[6] = (TileDefinition){105, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle03.png"), 1};

    defs_storage[7] = (TileDefinition){106, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle04.png"), 1};
    defs_storage[8] = (TileDefinition){107, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle05.png"), 1};
    defs_storage[9] = (TileDefinition){108, LoadTexture("assets/tiles/tree/big_tree/big_tree_middle06.png"), 1};

    //bottom
    defs_storage[10] = (TileDefinition){109, LoadTexture("assets/tiles/tree/big_tree/big_tree_bottom01.png"), 1};
    defs_storage[11] = (TileDefinition){110, LoadTexture("assets/tiles/tree/big_tree/big_tree_bottom02.png"), 1};
    defs_storage[12] = (TileDefinition){111, LoadTexture("assets/tiles/tree/big_tree/big_tree_bottom03.png"), 1};

    // arvore (3x2) - horizontal
    //top
    defs_storage[13] = (TileDefinition){120, LoadTexture("assets/tiles/tree/small_tree/small_tree_up01.png"), 1};
    defs_storage[14] = (TileDefinition){121, LoadTexture("assets/tiles/tree/small_tree/small_tree_up02.png"), 1};

    //middle
    defs_storage[15] = (TileDefinition){122, LoadTexture("assets/tiles/tree/small_tree/small_tree_middle01.png"), 1};
    defs_storage[16] = (TileDefinition){123, LoadTexture("assets/tiles/tree/small_tree/small_tree_middle02.png"), 1};

    //bottom
    defs_storage[17] = (TileDefinition){124, LoadTexture("assets/tiles/tree/small_tree/small_tree_bottom01.png"), 1};
    defs_storage[18] = (TileDefinition){125, LoadTexture("assets/tiles/tree/small_tree/small_tree_bottom02.png"), 1};

    // plantas basicas (1x1)
    //planta
    defs_storage[19] = (TileDefinition){200, LoadTexture("assets/tiles/plant/plant01.png"), 1};
    
    //flor
    defs_storage[20] = (TileDefinition){201, LoadTexture("assets/tiles/plant/flower01.png"), 0}; 

    // cerca
    //top
    defs_storage[21] = (TileDefinition){50, LoadTexture("assets/tiles/fence/fence_top01.png"), 1};
    defs_storage[22] = (TileDefinition){51, LoadTexture("assets/tiles/fence/fence_top02.png"), 1};
    defs_storage[23] = (TileDefinition){52, LoadTexture("assets/tiles/fence/fence_top03.png"), 1};

    //middle
    defs_storage[24] = (TileDefinition){53, LoadTexture("assets/tiles/fence/fence_middle01.png"), 1};
    defs_storage[25] = (TileDefinition){54, LoadTexture("assets/tiles/fence/fence_middle02.png"), 1};

    //bottom
    defs_storage[26] = (TileDefinition){55, LoadTexture("assets/tiles/fence/fence_bottom01.png"), 1};
    defs_storage[27] = (TileDefinition){56, LoadTexture("assets/tiles/fence/fence_bottom02.png"), 1};
    defs_storage[28] = (TileDefinition){57, LoadTexture("assets/tiles/fence/fence_bottom03.png"), 1};

    // terreno 
    defs_storage[29] = (TileDefinition){20, LoadTexture("assets/tiles/terrain/elevated_terrain/elevated_terrain01.png"), 1};
    defs_storage[30] = (TileDefinition){21, LoadTexture("assets/tiles/terrain/elevated_terrain/elevated_terrain02.png"), 1};
    defs_storage[31] = (TileDefinition){22, LoadTexture("assets/tiles/terrain/elevated_terrain/elevated_terrain03.png"), 1};

    // caminho de terra
    //fill
    defs_storage[32] = (TileDefinition){80, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_fill.png"), 0};

    //top
    defs_storage[33] = (TileDefinition){81, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_top01.png"), 0};
    defs_storage[34] = (TileDefinition){82, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_top02.png"), 0};

    //middle
    defs_storage[35] = (TileDefinition){83, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_middle01.png"), 0};
    defs_storage[36] = (TileDefinition){84, LoadTexture("assets/tiles/terrain/dirt_pat/dirt_path_middle02.png"), 0};

    //bottom
    defs_storage[37] = (TileDefinition){85, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_bottom01.png"), 0};
    defs_storage[38] = (TileDefinition){86, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_bottom02.png"), 0};

    //turn bottom
    defs_storage[39] = (TileDefinition){87, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_turn_btm01.png"), 0};
    defs_storage[40] = (TileDefinition){88, LoadTexture("assets/tiles/terrain/dirt_path_dirt_path_turn_btm02.png"), 0};

    //turn top
    defs_storage[41] = (TileDefinition){89, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_turn_top01.png"), 0};
    defs_storage[42] = (TileDefinition){90, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_turn_top02.png"), 0};

    //horizontal
    //top
    defs_storage[43] = (TileDefinition){91, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_hz_middle01.png"), 0};
    //bottom
    defs_storage[44] = (TileDefinition){92, LoadTexture("assets/tiles/terrain/dirt_path/dirt_path_hz_middle02.png"), 0};

    // arvore conitnua(vertical)
    //top
    defs_storage[45] = (TileDefinition){126, LoadTexture("assets/tiles/tree/small_tree/continuos_smalltree_top01.png"), 1};
    defs_storage[46] = (TileDefinition){127, LoadTexture("assets/tiles/tree/small_tree/continuos_smalltree_top02.png"), 1};

    //middle
    defs_storage[47] = (TileDefinition){128, LoadTexture("assets/tiles/tree/small_tree/continuos_smalltree_middle01.png"), 1};
    defs_storage[48] = (TileDefinition){129, LoadTexture("assets/tiles/tree/small_tree/continuos_smalltree_middle02.png"), 1};

    //bottom
    defs_storage[49] = (TileDefinition){130, LoadTexture("assets/tiles/tree/small_tree/continuos_smalltree_btm01.png"), 1};
    defs_storage[50] = (TileDefinition){131, LoadTexture("assets/tiles/tree/small_tree/continuos_smalltree_btm02.png"), 1};

    tile_defs = defs_storage;
    tile_defs_count = 51; // quantidade de tiles carregados
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

