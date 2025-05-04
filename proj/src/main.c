#include <lcom/lcf.h>
#include "controller/timer/timer.c"
#include "controller/timer/i8254.h"
#include "controller/video/gpu.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "view/view.h"
#include "config.h"


uint8_t timer_mask;
uint8_t keyboard_mask;
uint8_t mouse_mask;


extern uint8_t packet_byte;
struct packet pp;
uint8_t packet_idx = 0;


int (main)(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv)) return 1;
  lcf_cleanup();
  return 0;
}



int setup() {

  // Atualização da frequência
  if (timer_set_frequency(TIMER, GAME_FREQUENCY) != 0) return 1;

  if (set_frame_buffer(VIDEO_MODE) != 0) return 1;

  // Inicialização do modo gráfico
  if (set_graphic_mode(VIDEO_MODE) != 0) return 1;

  

  // Ativação das interrupções dos dispositivos
  if (timer_subscribe_int(&timer_mask) != 0) return 1;
  if (kbd_subscribe_int(&keyboard_mask) != 0) return 1;
  if (mouse_subscribe_int(&mouse_mask) != 0) return 1;

  if(mouse_set_data_reporting(true) != 0) return 1;

 

  return 0;
}

int reset() {

  // Volta ao modo de texto
  if (vg_exit() != 0) return 1;

  // Desativa todas as interrupções
  if (timer_unsubscribe_int() != 0) return 1;
  if (kbd_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

int (proj_main_loop)(int argc, char *argv[]) {

  if (setup() != 0) return teardown();

  // Tratamento das interrupções
  int ipc_status;
  message msg;
  while (true) {
    
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & timer_mask){

          }
          if (msg.m_notify.interrupts & keyboard_mask){
            kbd_int_handler();
            break;
          }
          if (msg.m_notify.interrupts & mouse_mask){
            mouse_int_handler();
            break;
          }
        }
    }
  }
  
  if (reset() != 0) return 1;

  return 0;
}