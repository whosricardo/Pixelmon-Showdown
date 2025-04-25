#include "player.h"

void update_player(Player *player, float delta) {
    float speed = 100.0f;
    Vector2 movement = {0, 0};

    if (IsKeyDown(KEY_RIGHT)) {
        movement.x += 1;
        player->direction = DIRECTION_RIGHT;
    } else if (IsKeyDown(KEY_LEFT)) {
        movement.x -= 1;
        player->direction = DIRECTION_LEFT;
    } else if (IsKeyDown(KEY_DOWN)) {
        movement.y += 1;
        player->direction = DIRECTION_DOWN;
    } else if (IsKeyDown(KEY_UP)) {
        movement.y -= 1;
        player->direction = DIRECTION_UP;
    }

    // Modo de movimento (ex: Shift = correr)
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        player->mode = MODE_RUN;
        speed = 160.0f;
    } else {
        player->mode = MODE_WALK;
    }

    if (movement.x != 0 || movement.y != 0) {
        player->frame_timer += delta;
        if (player->frame_timer >= player->animations.frame_duration) {
            player->current_frame = (player->current_frame + 1) % FRAME_COUNT;
            player->frame_timer = 0;
        }

        player->position.x += movement.x * speed * delta;
        player->position.y += movement.y * speed * delta;
    } else {
        player->current_frame = 1; // frame neutro parado
    }
}

void draw_player(const Player *player) {
    Texture2D sprite;

    switch (player->mode) {
        case MODE_WALK:
            sprite = player->animations.walk[player->direction][player->current_frame];
            break;
        case MODE_RUN:
            sprite = player->animations.run[player->direction][player->current_frame];
            break;
        case MODE_BIKE:
            sprite = player->animations.bike[player->direction][player->current_frame];
            break;
        case MODE_BIKE_ACRO:
            sprite = player->animations.bike_acro[player->direction][player->current_frame];
            break;
        case MODE_BATTLE:
            sprite = player->animations.battle[player->current_frame % 5];
            break;
    }

    DrawTexture(sprite, player->position.x, player->position.y, WHITE);
}
