#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t old_config;
  if (timer_get_conf(timer, &old_config)) return 1;

  uint8_t sel_timer;
  switch (timer) {
    case 0:
      sel_timer = TIMER_SEL0;
      break;
    case 1:
      sel_timer = TIMER_SEL1;
      break;
    case 2:
      sel_timer = TIMER_SEL2;
      break;
    default:
      return 1;
  }

  uint8_t new_config = sel_timer | TIMER_LSB_MSB | (old_config & 0xF);
  if (sys_outb(TIMER_CTRL, new_config)) return 1;

  switch (timer) {
    case 0:
      sel_timer = TIMER_0;
      break;
    case 1:
      sel_timer = TIMER_1;
      break;
    case 2:
      sel_timer = TIMER_2;
      break;
    default:
      return 1;
  }

  if (TIMER_FREQ < freq) return 1;
  uint16_t frequency = (TIMER_FREQ / freq) & 0xFFFF;
  
  uint8_t lsb;
  if (util_get_LSB(frequency, &lsb)) return 1;
  if (sys_outb(sel_timer, lsb)) return 1;

  uint8_t msb;
  if (util_get_MSB(frequency, &msb)) return 1;
  if (sys_outb(sel_timer, msb)) return 1;
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (!st) return 1;
  uint8_t ctrl = (BIT(1) >> timer) | BIT(5) | BIT(6) | BIT(7);
  if (sys_outb(TIMER_CTRL, ctrl)) return 1;
  if (util_sys_inb(TIMER_0 + timer, st)) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val val;
  switch(field) {
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      switch ((timer >> 4) & 0x2) {
        case 0:
          val.in_mode = INVAL_val; 
          break;
        case 1:
          val.in_mode = LSB_only;
          break;
        case 2:
          val.in_mode = MSB_only;
          break;
        case 3:
          val.in_mode = MSB_after_LSB;
          break;
        default:
          return 1;
      }
      break;
    case tsf_mode:
      val.count_mode = (timer >> 1) & 0x7;
      break;
    case tsf_base: 
      val.in_mode = timer & 1;
      break;
    default:
      return 1;
  }
  return timer_print_config(timer, field, val);
}
