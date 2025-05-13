#include <lcom/lcf.h>
#include "controller/timer/i8254.h"
#include "controller/video/gpu.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "view/view.h"
#include "config.h"

#include "model/game/game.h"
#include "model/game/maze.h"
#include "model/game/player.h"
#include "model/game/level.h"

uint8_t timer_mask;
uint8_t keyboard_mask;
uint8_t mouse_mask;

extern uint8_t *main_frame_buffer;
extern uint8_t *sec_frame_buffer;

extern uint8_t scan_code;
extern uint8_t packet_byte;
extern uint8_t packet[3];
struct packet pp;
uint8_t packet_idx = 0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  // lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int setup() {
  // set graphic mode
  if (set_frame_buffers(VIDEO_MODE) != 0) return 1;
  if (set_graphic_mode(VIDEO_MODE) != 0) return 1;
  
  // subscribe to interrupts
  if (timer_set_frequency(TIMER, GAME_FREQUENCY) != 0) return 1;
  if (timer_subscribe_int(&timer_mask) != 0) return 1;

  if (kbd_subscribe_int(&keyboard_mask) != 0) return 1;
  
  if (mouse_subscribe_int(&mouse_mask) != 0) return 1;
  if(mouse_set_data_reporting(true) != 0) return 1;

  return 0;
}

int reset() {
  // set text mode
  if (vg_exit() != 0) return 1;

  // unsubscribe from interrupts
  if (timer_unsubscribe_int() != 0) return 1;
  if (kbd_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {
  Game *game = create_game();

  setup();

  int ipc_status;
  message msg;
  while (scan_code != ESC_BREAK_CODE) {
    State state = get_state(game);
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & timer_mask){
            game_timer_handler(game);
          }
          if (msg.m_notify.interrupts & keyboard_mask){
            kbd_int_handler();
            if(state == LEVEL){
              keyboard_handler();
            }
            break;
          }
          if (msg.m_notify.interrupts & mouse_mask){
            mouse_int_handler();
            mouse_sync_packets(packet, &packet_idx, packet_byte);
            if(packet_idx == 3){
              mouse_build_packet(packet, &packet_idx, &pp);
              packet_idx = 0;
            }
            break;
          }
        }
    }
  }
  
  if (reset() != 0) return 1;

  destroy_game(game);
  return 0;
}
