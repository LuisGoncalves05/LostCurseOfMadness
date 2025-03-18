#include "kbd.h"
#include "i8042.h"
#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

uint32_t kbd_subscription_id = 1;
uint8_t kbd_continue = 1;
uint8_t scan_code;
uint32_t cnt = 0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t kbd_line_bit;
  if (kbd_subscribe_int(&kbd_line_bit))
    return 1;

  uint8_t scan_codes[2] = {0, 0};
  while (kbd_continue) {
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
            kbc_ih();
            bool make = (scan_code >> 7) & 1;

            bool print = true;
            uint8_t scan_code_size = 1;
            if (scan_codes[0] == TWO_BYTE_SCANCODE) {
              scan_codes[1] = scan_code;
              scan_code_size = 2;
              print = true;
            } else {
              scan_codes[0] = scan_code;
              print = (scan_codes[0] != TWO_BYTE_SCANCODE);
            }
            if (print) {
              kbd_print_scancode(make, scan_code_size, scan_codes);
              scan_codes[0] = 0;
            }
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

  return kbd_print_no_sysinb(cnt) | kbd_unsubcribe_int();
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}