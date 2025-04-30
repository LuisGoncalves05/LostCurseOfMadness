#include "../lab3/i8042.h"
#include "mouse.h"
#include <lcom/lcf.h>

extern uint8_t packet_byte;
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

int(mouse_test_packet)(uint32_t count) {
  uint8_t mouse_line_bit;
  if (mouse_subscribe_int(&mouse_line_bit))
    return 1;

  if (mouse_set_data_reporting(true))
    return 1;

  struct packet pp;
  uint8_t packet[3] = {0, 0, 0};
  uint8_t packet_idx = 0;

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
              if (mouse_build_packet(packet, &packet_idx, &pp))
                return 1;
              mouse_print_packet(&pp);
              count--;
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

  if (mouse_set_data_reporting(false))
    return 1;

  return mouse_unsubscribe_int();
}

int(mouse_test_async)(uint8_t idle_time) {
  uint8_t timer_line_bit;
  if (timer_subscribe_int(&timer_line_bit))
    return 1;

  uint8_t mouse_line_bit;
  if (mouse_subscribe_int(&mouse_line_bit))
    return 1;

  if (mouse_set_data_reporting(true))
    return 1;

  struct packet pp;
  uint8_t packet[3] = {0, 0, 0};
  uint8_t packet_idx = 0;

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
              if (mouse_build_packet(packet, &packet_idx, &pp))
                return 1;
              mouse_print_packet(&pp);
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

  if (mouse_set_data_reporting(false))
    return 1;

  if (mouse_unsubscribe_int())
    return 1;

  return timer_unsubscribe_int();
}


typedef enum { INITIAL,
               LINE_UP,
               TOP,
               LINE_DOWN,
               COMPLETE } state_t;

typedef enum { RIGHT_DOWN,
               RIGHT_UP,
               LEFT_DOWN,
               LEFT_UP,
               MOVE,
               INVALID } event_t;

typedef struct {
  int delta_x;
  int delta_y;
  bool right_down;
  bool left_down;
} line_t;

state_t state = INITIAL;
line_t line = {0, 0, 0, 0};

event_t(get_event)(struct packet *pp) {
  if (pp->mb)
    return INVALID;
  else if (pp->rb && !line.right_down)
    return RIGHT_DOWN;
  else if (!pp->rb && line.right_down)
    return RIGHT_UP;
  else if (pp->lb && !line.left_down)
    return LEFT_DOWN;
  else if (!pp->lb && line.left_down)
    return LEFT_UP;
  else if (!pp->x_ov && !pp->y_ov)
    return MOVE;
  else
    return INVALID;
}

bool line_up(uint8_t x_len) {
  double slope = (double) line.delta_y / line.delta_x;
  return slope > 1 && line.delta_x >= x_len;
}

bool line_down(uint8_t x_len) {
  double slope = (double) line.delta_y / line.delta_x;
  return slope < -1 && line.delta_x >= x_len;
}

void reset_state() {
  line.delta_x = 0;
  line.delta_y = 0;
  state = INITIAL;
}

void(r_down_handler)(uint8_t x_len, uint8_t tolerance, struct packet *pp) {
  line.delta_x = 0;
  line.delta_y = 0;
  line.right_down = 1;
  if (state == TOP)
    state = LINE_DOWN;
  else
    reset_state();
}

void(r_up_handler)(uint8_t x_len, uint8_t tolerance, struct packet *pp) {
  line.right_down = 0;
  if (state == LINE_DOWN && line_down(x_len))
    state = COMPLETE;
  else
    reset_state();
}

void(l_down_handler)(uint8_t x_len, uint8_t tolerance, struct packet *pp) {
  line.delta_x = 0;
  line.delta_y = 0;
  line.left_down = 1;
  if (state == INITIAL || state == TOP)
    state = LINE_UP;
  else
    reset_state();
}

void(l_up_handler)(uint8_t x_len, uint8_t tolerance, struct packet *pp) {
  line.left_down = 0;
  if (state == LINE_UP && line_up(x_len))
    state = TOP;
  else
    reset_state();
}

void(move_handler)(uint8_t x_len, uint8_t tolerance, struct packet *pp) {
  if (state == INITIAL)
    return;
  if (state == TOP) {
    if (pp->delta_x + tolerance < 0) {
      reset_state();
      printf("tolerance %d surpassed by %d\n", tolerance, pp->delta_x);
    }
    if (pp->delta_x > tolerance) {
      reset_state();
      printf("tolerance %d surpassed by %d here!!!\n", tolerance, pp->delta_x);
    }
    if (pp->delta_y + tolerance < 0) {
      reset_state();
      printf("tolerance %d surpassed by %d\n", tolerance, pp->delta_x);
    }
    if (pp->delta_y > tolerance) {
      reset_state();
      printf("tolerance %d surpassed by %d\n", tolerance, pp->delta_x);
    }
  }
  else if (state == LINE_UP) {
    if (pp->delta_x < tolerance * (-1))
      reset_state();
    else if (pp->delta_y < tolerance * (-1))
      reset_state();
    else {
      line.delta_x += pp->delta_x;
      line.delta_y += pp->delta_y;
    }
  }
  else if (state == LINE_DOWN) {
    if (pp->delta_x < tolerance * (-1))
      reset_state();
    else if (pp->delta_y > tolerance)
      reset_state();
    else {
      line.delta_x += pp->delta_x;
      line.delta_y += pp->delta_y;
    }
  }
}

void (*event_handlers[])(uint8_t x_len, uint8_t tolerance, struct packet *pp) = {
  r_down_handler,
  r_up_handler,
  l_down_handler,
  l_up_handler,
  move_handler
};

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t mouse_line_bit;
  if (mouse_subscribe_int(&mouse_line_bit))
    return 1;

  if (mouse_set_data_reporting(true))
    return 1;

  struct packet pp;
  uint8_t packet[3] = {0, 0, 0};
  uint8_t packet_idx = 0;

  int ipc_status, r;
  message msg;
  while (state != COMPLETE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status))) {
      printf("driver_receive failed with: %d", r);
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
              if (mouse_build_packet(packet, &packet_idx, &pp))
                return 1;
              mouse_print_packet(&pp);
              event_t event = get_event(&pp);
              if (event == INVALID)
                reset_state();
              else
                event_handlers[event](x_len, tolerance, &pp);
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (mouse_set_data_reporting(false))
    return 1;
  return mouse_unsubscribe_int();
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  /* This year you need not implement this. */
  printf("%s(%u, %u): under construction\n", __func__, period, cnt);
  return 1;
}
