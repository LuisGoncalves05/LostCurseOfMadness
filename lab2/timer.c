#include "i8254.h"
#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

int32_t timer_hook_id = TIMER0_IRQ;
uint32_t interrupt_counter = 0;


int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t old_config;
  if (timer_get_conf(timer, &old_config))
    return 1;

  uint8_t selected_timer;
  switch (timer) {
    case 0:
      selected_timer = TIMER_SEL0;
      break;
    case 1:
      selected_timer = TIMER_SEL1;
      break;
    case 2:
      selected_timer = TIMER_SEL2;
      break;
    default:
      return 1;
  }

  /*
  selected timer set in bits 6 and 7 
  sets up counter initialization
  keeps the 4 least significant bits the same
  */
  uint8_t new_config = selected_timer | TIMER_LSB_MSB | (old_config & 0xF);
  if (sys_outb(TIMER_CTRL, new_config))
    return 1;

  switch (timer) {
    case 0:
      selected_timer = TIMER_0;
      break;
    case 1:
      selected_timer = TIMER_1;
      break;
    case 2:
      selected_timer = TIMER_2;
      break;
    default:
      return 1;
  }

  /*
  frequency must be stored in a 16 bit unsigned integer so it cannot be more than 0xFFFF
  and it must also be positive and therefore 
  TIMER_FREQ / freq > 0xFFFF <=> freq < TIMER_FREQ / 0xFFFF <=> freq < 18.2
  TIMER_FREQ / freq < 1 <=> freq > TIMER_FREQ
  */
  if (freq < 19 || freq > TIMER_FREQ)
    return 1;
  
  uint16_t frequency = TIMER_FREQ / freq;

  uint8_t lsb;
  if (util_get_LSB(frequency, &lsb))
    return 1;
  if (sys_outb(selected_timer, lsb))
    return 1;

  uint8_t msb;
  if (util_get_MSB(frequency, &msb))
    return 1;
  return sys_outb(selected_timer, msb);
}


int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  *bit_no = BIT(timer_hook_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
}


int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id);
}


void (timer_int_handler)() {
  interrupt_counter++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *status) {
  if (status == 0)
    return 1;

  uint8_t ctrl = TIMER_RB_SEL(timer) | TIMER_RB_COUNT_ | TIMER_RB_COMMAND;
  if (sys_outb(TIMER_CTRL, ctrl))
    return 1;

  uint8_t timer_count_register;
  switch (timer) {
    case 0:
      timer_count_register = TIMER_0;
      break;
    case 1:
      timer_count_register = TIMER_1;
      break;
    case 2:
      timer_count_register = TIMER_2;
      break;
    default:
      return 1;
  }
  return util_sys_inb(timer_count_register, status);
}


int (timer_display_conf)(uint8_t timer, uint8_t status, enum timer_status_field field) {
  union timer_status_field_val val;
  switch (field) {
    case tsf_all:
      val.byte = status;
      break;
    case tsf_initial:
      switch (status & TIMER_LSB_MSB) {
        case 0:
          val.in_mode = INVAL_val;
          break;
        case TIMER_LSB:
          val.in_mode = LSB_only;
          break;
        case TIMER_MSB:
          val.in_mode = MSB_only;
          break;
        case TIMER_LSB_MSB:
          val.in_mode = MSB_after_LSB;
          break;
        default:
          return 1;
      }
      break;
    case tsf_mode:
      val.count_mode = (status & TIMER_COUNTING_MODE) >> 1;
      break;
    case tsf_base:
      val.in_mode = (status & TIMER_BCD) == TIMER_BCD ? TIMER_BCD : TIMER_BIN;
      break;
    default:
      return 1;
  }

  return timer_print_config(timer, field, val);
}
