#ifndef KBD_H
#define KBD_H

#include <stdbool.h>
#include <stdint.h>

/** @defgroup kbd kbd
 * @{
 *
 * Functions for using the i8042 kbd
 */


/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_unsubscribe_int)(void);

/**
 * @brief Keyboard interrupt handler
 *
 * Some desc
 */
void (kbc_ih)(void);


#endif /* __kbd_H */
