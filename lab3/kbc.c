#include "kbc.h"
#include <lcom/lcf.h>

int kbc_write_byte(uint8_t port, uint8_t byte) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    uint8_t status;
    if (util_sys_inb(KBC_STATUS_REGISTER, &status))
      return 1;

    // Check for timeout or parity errors and if input buffer is empty
    if ((status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT)) == 0 && !(status & KBC_INPUT_BUFFER_FULL))
      return sys_outb(port, byte);

    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  }

  return 1;
}

int kbc_write_command(uint8_t command) {
  return kbc_write_byte(KBC_COMMAND_REGISTER, command);
}

int kbc_write_command_arguments(uint8_t argument) {
  return kbc_write_byte(KBC_ARGUMENT_REGISTER, argument);
}

int kbc_read_byte(uint8_t port, uint8_t* byte, bool mouse) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    uint8_t status;
    if (util_sys_inb(KBC_STATUS_REGISTER, &status))
      return 1;

    // Check for timeout or parity errors and if output buffer is full
    if ((status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT)) == 0 && (status & KBC_OUTPUT_BUFFER_FULL)) {
      // Check if request asks for the data from the same device it comes from
      if ((mouse && !(status & KBC_AUX_BIT)) || (!mouse && (status & KBC_AUX_BIT)))
        return 1;

      return util_sys_inb(port, byte);
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  }

  return 1;
}

int kbc_read_command_return(uint8_t* retv, bool mouse) {
  return kbc_read_byte(KBC_RETURN_REGISTER, retv, mouse);
}
