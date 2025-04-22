#include "../lab3/kbc.h"
#include "mouse.h"

int32_t mouse_hook_id = 2;
extern uint8_t packet_byte;


int (mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(mouse_hook_id);
  return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}


int (mouse_unsubscribe_int)(void) {
  return sys_irqrmpolicy(&mouse_hook_id);
}


void (mouse_int_handler)(void) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    if (mouse_read_packet() == 0) return;
    attempts++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
}

void (mouse_ih)(void) {
  mouse_int_handler();
}


int (mouse_read_packet)(void) {
  uint8_t status;
  if (util_sys_inb(KBC_STATUS_REGISTER, &status)) 
    return 1;

  if (kbc_read_command_return(&packet_byte, true)) 
    return 1;

  return !(status & KBC_OUTPUT_BUFFER_FULL) || (status & (KBC_TIMEOUT_BIT | KBC_PARITY_BIT));
}


int (mouse_sync_packets)(uint8_t* packet, uint8_t* packet_idx, uint8_t packet_byte) {
  if (*packet_idx > 2)
    return 1;

  if (*packet_idx == 0) {
    if (packet_byte & MOUSE_SYNC) {
      packet[*packet_idx] = packet_byte;
      (*packet_idx)++;
    }
  } else {
    packet[*packet_idx] = packet_byte;
    (*packet_idx)++;
  }
  
  return 0;
}


int (mouse_handle_packet)(uint8_t* packet, uint8_t* packet_idx) {
  if (packet == NULL || packet_idx == NULL)
    return 1;

  uint8_t b1 = packet[0], b2 = packet[1], b3 = packet[2];

  struct packet pp;

  pp.bytes[0] = b1;
  pp.bytes[1] = b2;
  pp.bytes[2] = b3;

  pp.lb = b1 & MOUSE_LB;
  pp.mb = b1 & MOUSE_MB;
  pp.rb = b1 & MOUSE_RB;

  pp.x_ov = b1 & MOUSE_X_OVERFLOW;
  pp.y_ov = b1 & MOUSE_Y_OVERFLOW;

  pp.delta_x = (b1 & MOUSE_X_SIGN) ? (int16_t)(b2 | 0xFF00) : b2;
  pp.delta_y = (b1 & MOUSE_Y_SIGN) ? (int16_t)(b3 | 0xFF00) : b3;

  mouse_print_packet(&pp);
  (*packet_idx) = 0;

  return 0;
}


int (mouse_set_data_reporting)(bool b) {
  uint8_t attempts = 0;

  while (attempts < READ_ATTEMPT_LIMIT) {
    if (kbc_write_command(MOUSE_WRITE_COMMAND))
      return 1;

    if (kbc_write_command_arguments(b ? MOUSE_ENABLE_DATA_REPORTING : MOUSE_DISABLE_DATA_REPORTING))
      return 1;

    tickdelay(micros_to_ticks(DELAY_US));

    uint8_t status;
    if (kbc_read_command_return(&status, true))
      return 1;

    if (status == MOUSE_ACK)
      return 0;

    attempts++;
  }

  return 1;
}
