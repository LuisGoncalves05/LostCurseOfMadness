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
    clear(sec_frame_buffer);
    draw_player();
    copy_frame_buffer();
}
