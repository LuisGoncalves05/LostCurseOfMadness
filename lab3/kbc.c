#include "kbc.h"
#include <lcom/lcf.h>

int32_t kbd_hood_id = KBD_IRQ;
extern uint32_t interrupt_counter;
extern bool kbd_continue;
extern uint8_t scan_code;


int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(kbd_hood_id);
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hood_id);
}


int (kbd_unsubscribe_int)(void) {
  return sys_irqrmpolicy(&kbd_hood_id);
}


void (kbd_int_handler)(void) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    if (kbd_read_scan_code() == 0) 
      return;
    attempts++;
    tickdelay(micros_to_ticks(DELAY_US));
  }

  kbd_continue = false;
}


int (kbd_read_scan_code)(void) {
  uint8_t status;
  if (util_sys_inb(KBC_STATUS_REGISTER, &status)) 
    return 1;

  uint8_t output_buffer_full = status & KBC_OUTPUT_BUFFER_FULL;

  if (util_sys_inb(KBC_OUTPUT_BUFFER, &scan_code)) 
    return 1;

  return !output_buffer_full || (status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT));
}


void (kbd_handle_scan_code)(uint8_t* scan_codes, bool timed) {
  if (scan_code == ESC_BREAK_CODE) {
    kbd_continue = false;
  }

  bool make = (scan_code & BREAK) == 0; 
  bool print = false;

  uint8_t scan_code_size;
  if (scan_codes[0] == TWO_BYTE_SCANCODE) {
    scan_codes[1] = scan_code;
    scan_code_size = 2;
    print = true;
  } else {
    scan_codes[0] = scan_code;
    scan_code_size = 1;
    print = (scan_codes[0] != TWO_BYTE_SCANCODE);
  }
  
  if (print) {
    kbd_print_scancode(make, scan_code_size, scan_codes);
    scan_codes[0] = 0;
    scan_codes[1] = 0;
    if (timed) 
      interrupt_counter = 0;
  }
}


int (kbc_write_byte)(uint8_t port, uint8_t byte) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    uint8_t status;
    if (util_sys_inb(KBC_STATUS_REGISTER, &status))
      return 1;

    uint8_t good_read = ((status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT)) == 0);
    uint8_t input_buffer_full = status & KBC_INPUT_BUFFER_FULL;

    if (good_read && !input_buffer_full)
      return sys_outb(port, byte);

    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  }

  return 1;
}


int (kbc_write_command)(uint8_t command) {
  return kbc_write_byte(KBC_COMMAND_REGISTER, command);
}


int (kbc_write_command_arguments)(uint8_t argument) {
  return kbc_write_byte(KBC_ARGUMENT_REGISTER, argument);
}


int (kbc_read_byte)(uint8_t port, uint8_t* byte) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    uint8_t status;
    if (util_sys_inb(KBC_STATUS_REGISTER, &status))
      return 1;

    uint8_t good_read = ((status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT)) == 0);
    uint8_t output_buffer_full = status & KBC_OUTPUT_BUFFER_FULL;

    if (good_read && output_buffer_full)
      return util_sys_inb(port, byte);

    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  } 

  return 1;
}


int (kbc_read_command_return)(uint8_t* retv) {
 return kbc_read_byte(KBC_RETURN_REGISTER, retv);
}
