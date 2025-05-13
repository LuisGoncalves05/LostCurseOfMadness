#include "game.h"
#include "controller/video/gpu.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/timer/i8254.h"
#include "view/view.h"
#include "model/Sprite.h"


extern uint8_t scan_code;
extern uint8_t *sec_frame_buffer;
extern uint8_t *main_frame_buffer;

Player *player;
Sprite *player_sprite;


void main_game_loop(){
    clear(sec_frame_buffer);
    draw_player(&player, &player_sprite);
    copy_frame_buffer();
}



void keyboard_handler() {
    if (player == NULL) return; // Avoid NULL dereference

    bool moved = 0;

    switch (scan_code) {
        case KEY_W:
            player->sprite->yspeed += player->acceleration;
            playerIsAtMaxSpeed(player);
            player->sprite->y -= player->sprite->yspeed;
            moved = 1;
            break;

        case KEY_A:
            player->sprite->xspeed += player->acceleration;
            playerIsAtMaxSpeed(player);
            player->sprite->x -= player->sprite->xspeed;
            moved = 1;
            break;

        case KEY_S:
            player->sprite->yspeed += player->acceleration;
            playerIsAtMaxSpeed(player);
            player->sprite->y += player->sprite->yspeed;
            moved = 1;
            break;

        case KEY_D:
            player->sprite->xspeed += player->acceleration;
            playerIsAtMaxSpeed(player);
            player->sprite->x += player->sprite->xspeed;
            moved = 1;
            break;

        default:
            break;
    }

    player->moved = moved;
    playerStopped(player);
}

void mouse_handler(struct packet pp){
    
}
