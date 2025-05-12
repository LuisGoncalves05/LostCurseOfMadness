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

Player *player = NULL;
Sprite *player_sprite;


void main_game_loop(){
    clear(sec_frame_buffer);
    draw_player();
    copy_frame_buffer();
}

int draw_player(){

    if(player == NULL){
        player_sprite = create_sprite((xpm_map_t) penguin, 200, 200, 10, 10);
        player = create_player(HEALTH, player_sprite);
        vga_draw_rectangle(0, 0, 100, 100, 0x01, sec_frame_buffer);
    }

    draw_sprite_xpm(player->sprite, player->sprite->x, player->sprite->y);
    

    return 0;
}


void keyboard_handler(){
    bool moved =0;
    if(scan_code == KEY_W){
        player->sprite->y -= player->sprite->yspeed;
        moved = 1;
        
    }
    else if(scan_code == KEY_A){
        player->sprite->x -=  player->sprite->xspeed;
        moved = 1;
    }
    else if(scan_code == KEY_S){
        player->sprite->y +=  player->sprite->yspeed;
        moved = 1;
    }
    else if(scan_code == KEY_D){
        player->sprite->x +=  player->sprite->xspeed;
        moved = 1;
    }
    player->moved = moved;
    
}

void mouse_handler(struct packet pp){

}
