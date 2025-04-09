#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "../lab3/i8042.h"
#include "../lab3/kbc.h"
#include "mouse.h"

uint8_t packet_byte;
uint8_t packet_idx = 0;
extern uint32_t interrupt_counter;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  // lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t count) {
  uint8_t mouse_line_bit;
  if (mouse_subscribe_int(&mouse_line_bit))
      return 1;

  if (mouse_set_data_reporting(true))
      return 1;
  
  uint8_t packet[3] = {0, 0, 0};
  while (count) {
    /* Get a request message. */
    message msg;
    int ret, ipc_status;
    if ((ret = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", ret);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                    /* hardware interrupt notification */
          if (msg.m_notify.interrupts & mouse_line_bit) { /* subscribed interrupt */
            mouse_ih();
            if (mouse_sync_packets(packet, &packet_idx, packet_byte))
              return 1;
            if (packet_idx == 3) {
              if (mouse_handle_packet(packet, &packet_idx))
                return 1;
              count--;
            }
          }
          break;
        default:
            break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
              /* no standard messages expected: do nothing */
    }
  }
  
  if (mouse_set_data_reporting(false))
      return 1;

  return mouse_unsubscribe_int();
}


int (mouse_test_async)(uint8_t idle_time) {
  uint8_t timer_line_bit;
  if (timer_subscribe_int(&timer_line_bit))
    return 1;

  uint8_t mouse_line_bit;
  if (mouse_subscribe_int(&mouse_line_bit))
      return 1;

  if (mouse_set_data_reporting(true))
      return 1;
  
  uint8_t packet[3] = {0, 0, 0};
  while (interrupt_counter < sys_hz() * idle_time) {
    /* Get a request message. */
    message msg;
    int ret, ipc_status;
    if ((ret = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", ret);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                    /* hardware interrupt notification */
          if (msg.m_notify.interrupts & timer_line_bit) { /* subscribed interrupt */
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & mouse_line_bit) { /* subscribed interrupt */
            interrupt_counter = 0;
            mouse_ih();
            if (mouse_sync_packets(packet, &packet_idx, packet_byte))
              return 1;
            if (packet_idx == 3) {
              if (mouse_handle_packet(packet, &packet_idx))
                return 1;
            }
          }
          break;
        default:
            break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
              /* no standard messages expected: do nothing */
    }
  }
  
  if (mouse_set_data_reporting(false))
    return 1;

  if (mouse_unsubscribe_int()) 
    return 1;
    
  return timer_unsubscribe_int();
}


int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}


int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
