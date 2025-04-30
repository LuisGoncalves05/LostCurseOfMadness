#include <lcom/lcf.h>
#include <stdint.h>

uint32_t sys_inb_counter = 0;


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) {
    fprintf(stderr, "Invalid argument: lsb is NULL.\n");
    return 1;
  }
  *lsb = val & 0xFF;
  return 0;
}


int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) {
    fprintf(stderr, "Invalid argument: msb is NULL.\n");
    return 1;
  }
  *msb = (val >> 8) & 0xFF;
  return 0;
}


int(util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL) {
    fprintf(stderr, "Invalid argument: value is NULL.\n");
    return 1;
  }

  #ifdef LAB3
  sys_inb_counter++;
  #endif

  uint32_t val;
  if (sys_inb(port, &val) != 0) {
    fprintf(stderr, "Error reading from port 0x%x.\n", port);
    return 1;
  }

  *value = val & 0xFF;
  return 0;
}
