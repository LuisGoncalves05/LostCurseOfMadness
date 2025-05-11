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
    //Sprite *sprite = create_sprite();
    //Player *player = create_player(3, sprite);
    draw_player();
}

int draw_player(){
    
    vga_draw_rectangle(50, 50, 20, 20, 0xFF, sec_frame_buffer);
    copy_frame_buffer();
    return 0;
}

