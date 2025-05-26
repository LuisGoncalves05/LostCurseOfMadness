#include <lcom/lcf.h>
#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    if (lsb == NULL) {
        printf("util_get_LSB: NULL pointer provided.\n");
        return 1;
    }
    *lsb = val & 0xFF;
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if (msb == NULL) {
        printf("util_get_MSB: NULL pointer provided.\n");
        return 1;
    }
    *msb = (val >> 8) & 0xFF;
    return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
    if (value == NULL) {
        printf("util_sys_inb: NULL pointer provided.\n");
        return 1;
    }
    uint32_t val;
    if (sys_inb(port, &val) != 0) {
        printf("util_sys_inb: sys_inb failed.\n");
        return 1;
    }

    *value = val & 0xFF;
    return 0;
}
