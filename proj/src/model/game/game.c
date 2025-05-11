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



void main_game_loop(){
    Sprite *sprite = create_sprite((xpm_map_t) player, 50, 50, 0, 0);
    Player *player = create_player(3, sprite);
    printf("Player created with health: %d\n", player->health);
    draw_sprite_xpm(player->sprite, 0, 0);
    draw_player();
}

int draw_player(){
    
    
    copy_frame_buffer();
    return 0;
}

