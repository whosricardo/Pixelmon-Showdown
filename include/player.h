#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

#define FRAME_COUNT 3
#define DIRECTION_COUNT 4

typedef enum {
    DIRECTION_DOWN,
    DIRECTION_UP,
    DIRECTION_RIGHT,
    DIRECTION_LEFT
} Direction;

typedef enum {
    MODE_WALK,
    MODE_RUN,
    MODE_BIKE,
    MODE_BIKE_ACRO,
    MODE_BATTLE
} MovementMode;

typedef struct {
    Texture2D walk[DIRECTION_COUNT][FRAME_COUNT];
    Texture2D run[DIRECTION_COUNT][FRAME_COUNT];
    Texture2D bike[DIRECTION_COUNT][FRAME_COUNT];
    Texture2D bike_acro[DIRECTION_COUNT][FRAME_COUNT];
    Texture2D battle[5];
    float frame_duration;
} PlayerAnimations;

typedef struct {
    Vector2 position;
    Direction direction;
    MovementMode mode;
    int current_frame;
    float frame_timer;
    PlayerAnimations animations;
} Player;

void update_player(Player *player, float delta);
void draw_player(const Player *player);

#endif
