#include <lcom/lcf.h>

extern uint32_t interrupt_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  // lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  // lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t status;
  if (timer_get_conf(timer, &status))
    return 1;

  return timer_display_conf(timer, status, field);
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  return timer_set_frequency(timer, freq);
}

int(timer_test_int)(uint8_t time) {
  uint8_t timer_line_bit;
  if (timer_subscribe_int(&timer_line_bit))
    return 1;

  while (interrupt_counter < sys_hz() * time) {
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
            if (interrupt_counter % sys_hz() == 0) /* Every HZ interrupts a second passes */
              timer_print_elapsed_time();
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

  return timer_unsubscribe_int();
}
