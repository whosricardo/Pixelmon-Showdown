#ifndef PLAYER_LOADER_H
#define PLAYER_LOADER_H

typedef struct {
    char **down;
    char **up;
    char **left;
    char **right;
    int frame_count;
} DirectionalAnimation;

typedef struct {
    char **frames;
    int frame_count;
} SimpleAnimation;

typedef struct {
    DirectionalAnimation walk;
    DirectionalAnimation run;
    DirectionalAnimation bike;
    DirectionalAnimation bike_acro;
    SimpleAnimation battle;
    float frame_duration;
} PlayerAnimations;

PlayerAnimations *load_player_animations(const char *path);
void free_player_animations(PlayerAnimations *animations);

#endif
