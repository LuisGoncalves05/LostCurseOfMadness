#include "../lab3/keyboard.h"
#include "gpu.h"
#include <lcom/lcf.h>

extern uint8_t scan_code;
extern uint32_t interrupt_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  // lcf_trace_calls("/home/lcom/labs/grupo_2leic03_3/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/grupo_2leic03_3/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(wait_esc_press)() {
  uint8_t kbd_line_bit;
  if (kbd_subscribe_int(&kbd_line_bit))
    return 1;

  while (scan_code != ESC_BREAK_CODE) {
    /* Get a request message. */
    message msg;
    int ret, ipc_status;
    if ((ret = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", ret);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                  /* hardware interrupt notification */
          if (msg.m_notify.interrupts & kbd_line_bit) { /* subscribed interrupt */
            kbd_int_handler();
            kbd_handle_scan_code();
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  return kbd_unsubscribe_int();
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (vg_set_vbe_mode(mode))
    return 1;

  sleep(delay);

  return vg_exit();
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if (vg_init(mode) == NULL)
    return 1;

  if (vg_draw_rectangle(x, y, width, height, color)) {
    printf("tried to draw outside the screen");
  }

  if (wait_esc_press())
    return 1;

  return vg_exit();
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (vg_init(mode) == NULL)
    return 1;

  if (vg_draw_pattern(no_rectangles, first, step))
    return 1;

  if (wait_esc_press())
    return 1;

  return vg_exit();
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (vg_init(VBE_MODE1) == NULL)
    return 1;

  if (vg_draw_xpm(xpm, x, y))
    return 1;

  if (wait_esc_press())
    return 1;

  return vg_exit();
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  if (vg_init(VBE_MODE1) == NULL)
    return 1;

  uint8_t timer_line_bit;
  if (timer_subscribe_int(&timer_line_bit))
    return 1;

  uint8_t kbd_line_bit;
  if (kbd_subscribe_int(&kbd_line_bit))
    return 1;

  int8_t dir = get_direction(xi, xf);
  bool movement_x = (dir != 0);
  if (!movement_x)
    dir = get_direction(yi, yf);

  Sprite *sprite = create_sprite(xpm, xi, yi);

  if (vg_draw_sprite(sprite))
    return 1;

  while (scan_code != ESC_BREAK_CODE) {
    /* Get a request message. */
    message msg;
    int ret, ipc_status;
    if ((ret = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", ret);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                  /* hardware interrupt notification */
          if (msg.m_notify.interrupts & kbd_line_bit) { /* subscribed interrupt */
            kbd_int_handler();
            kbd_handle_scan_code();
          }
          if (msg.m_notify.interrupts & timer_line_bit) { /* subscribed interrupt */
            timer_int_handler();
            if (xi == xf && yi == yf)
              continue;
            if (interrupt_counter % (sys_hz() / fr_rate) == 0)
              if (vg_update_sprite(sprite, xf, yf, speed, dir, movement_x))
                return 1;
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  if (timer_unsubscribe_int())
    return 1;

  if (kbd_unsubscribe_int())
    return 1;

  return vg_exit();
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
