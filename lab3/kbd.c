#include "kbd.h"
#include "i8042.h"
#include <lcom/lcf.h>

extern uint32_t kbd_subscription_id;
extern uint8_t kbd_continue;
extern uint32_t cnt;
extern uint8_t scan_code;

#define DELAY_US 20000

int(kbd_subscribe_int)(uint8_t *bit_no) {
  if (!bit_no)
    return 1;
  bit_no = BIT(kbd_subscription_id);
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE || IRQ_EXCLUSIVE, kbd_subscription_id);
}

int(kbd_unsubscribe_int)(void) {
  return sys_irqrmpolicy(kbd_subscription_id);
}

void(kbc_ih)(void) {
  char attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    if (!kbd_try_read_scan_code())
      return;
    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  }
}

int(kbd_try_read_scan_code)(void) {
  uint8_t status;
  if (util_sys_inb(KBD_STATUS_REGISTER, &status))
    return 1;
  uint8_t good_read = (status & (KBD_TIMEOUT_BIT | KBD_PARITY_BIT) == 0);
  uint8_t output_buffer_full = status & OUTPUT_BUFFER_FULL;

  if (output_buffer_full)
    if (util_sys_inb(KBD_OUTPUT_BUFFER, &scan_code))
      return 1;

  return (good_read && output_buffer_full) ? 0 : 1;
}

/*
int write_command(uint8_t port, uint8_t command) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    uint8_t status;
    if (util_sys_inb(KBD_STATUS_REGISTER, &status))
      return 1;

    uint8_t good_read = (status & (KBD_TIMEOUT_BIT | KBD_PARITY_BIT) == 0);
    uint8_t output_buffer_full = status & OUTPUT_BUFFER_FULL;

    if (good_read && !output_buffer_full) {
      if (sys_outb(port, command))
        return 1;
      if (good_read)
        return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  }

  return 1;
}

int read_command(uint8_t port, uint8_t* command) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    uint8_t status;
    if (util_sys_inb(KBD_STATUS_REGISTER, &status))
      return 1;

    uint8_t good_read = (status & (KBD_TIMEOUT_BIT | KBD_PARITY_BIT) == 0);
    uint8_t output_buffer_full = status & OUTPUT_BUFFER_FULL;

    if (good_read && !output_buffer_full) {
      if (sys_outb(port, command))
        return 1;
      if (good_read)
        return 0;
    }

    tickdelay(micros_to_ticks(DELAY_US));
    attempts++;
  }

  return 1;
}
*/