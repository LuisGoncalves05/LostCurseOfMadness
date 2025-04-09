#include "kbc.h"
#include <lcom/lcf.h>


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


int (kbc_read_byte)(uint8_t port, uint8_t* byte, bool mouse) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    uint8_t status;
    if (util_sys_inb(KBC_STATUS_REGISTER, &status))
      return 1;

    uint8_t good_read = ((status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT)) == 0);
    uint8_t output_buffer_full = status & KBC_OUTPUT_BUFFER_FULL;

    if (good_read && output_buffer_full) {
      // finds a contradiction between data available and what is wanted
      if (
        (mouse && !(status & MOUSE_DATA)) ||
        (!mouse && (status & MOUSE_DATA))
      )
        return 1;

      return util_sys_inb(port, byte);
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  } 

  return 1;
}


int (kbc_read_command_return)(uint8_t* retv, bool mouse) {
  return kbc_read_byte(KBC_RETURN_REGISTER, retv, mouse);
}
