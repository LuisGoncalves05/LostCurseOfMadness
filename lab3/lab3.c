#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.h"
#include "keyboard.h"

extern bool esc_detected;
extern bool two_byte;
extern uint8_t scan_code;
extern uint32_t sys_inb_counter;
extern uint32_t interrupt_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  // lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/lab3/output.txt");

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
            if (kbd_handle_scan_code()) {
              uint8_t scan_codes[2] = {two_byte ? TWO_BYTE_SCANCODE : scan_code, scan_code};
              kbd_print_scancode(((scan_code & BREAK) == 0), two_byte ? 2 : 1, scan_codes);
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

  if (kbd_print_no_sysinb(sys_inb_counter))
    return 1;

  return kbd_unsubscribe_int();
}

int(kbd_test_poll)() {
  while (scan_code != ESC_BREAK_CODE) {
    if (!kbd_read_scan_code()) {
      if (kbd_handle_scan_code()) {
        uint8_t scan_codes[2] = {two_byte ? TWO_BYTE_SCANCODE : scan_code, scan_code};
        kbd_print_scancode(((scan_code & BREAK) == 0), two_byte ? 2 : 1, scan_codes);
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  if (kbd_print_no_sysinb(sys_inb_counter))
    return 1;

  if (kbc_write_command(KBC_READ_COMMAND_BYTE))
    return 1;

  uint8_t command_byte;
  if (kbc_read_command_return(&command_byte, false))
    return 1;

  command_byte |= KBD_INTERRUPTS_ON;

  if (kbc_write_command(KBC_WRITE_COMMAND_BYTE))
    return 1;

  return kbc_write_command_arguments(command_byte);
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t timer_line_bit;
  if (timer_subscribe_int(&timer_line_bit))
    return 1;

  uint8_t kbd_line_bit;
  if (kbd_subscribe_int(&kbd_line_bit))
    return 1;

  while (scan_code != ESC_BREAK_CODE && interrupt_counter < sys_hz() * n) {
    /* Get a request message. */
    message msg;
    int ret, ipc_status;
    if ((ret = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      fprintf(stderr, "driver_receive failed with: %d", ret);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                    /* hardware interrupt notification */
          if (msg.m_notify.interrupts & timer_line_bit) { /* subscribed interrupt */
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & kbd_line_bit) { /* subscribed interrupt */
            kbd_int_handler();
            if (kbd_handle_scan_code()) {
              uint8_t scan_codes[2] = {two_byte ? TWO_BYTE_SCANCODE : scan_code, scan_code};
              kbd_print_scancode(((scan_code & BREAK) == 0), two_byte ? 2 : 1, scan_codes);
            }
            interrupt_counter = 0;
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

  if (kbd_print_no_sysinb(sys_inb_counter))
    return 1;

  if (kbd_unsubscribe_int())
    return 1;

  return timer_unsubscribe_int();
}
