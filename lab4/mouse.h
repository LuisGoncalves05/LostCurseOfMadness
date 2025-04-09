#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>
#include "../lab3/i8042.h"

#define DELAY_US 20000
extern int32_t kbd_subscription_id;

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the mouse
 */

/**
 * @brief Subscribes and enables Mouse interrupts
 *
 * @param bit_no Will hold the bit (BIT(hook_id)) to be set in the mask returned upon an interrupt
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)(void);

/**
 * @brief Keyboard interrupt handler
 * 
 * Tries to read a packet byte from the kbc, if any error 
 * occurs tries again until READ_ATTEMPT_LIMIT
 */
void mouse_int_handler(void);

/**
 * @brief Reads packets from the Mouse
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_read_packet)(void);


/**
 * @brief Reads packets from the Mouse
 *
 * @param packet Packet array to be synced
 * 
 * @param packet_idx Index in the packet array
 * 
 * @param packet_byte Last packet byte read from the mouse
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_sync_packets)(uint8_t* packet, uint8_t* packet_idx, uint8_t packet_byte);


/**
 * @brief Handles (prints) packets read from the Mouse
 * 
 * @param packet Array that stores packet bytes
 * 
 * @param packet_idx Index in the packet array
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_handle_packet)(uint8_t* packet, uint8_t* packet_idx);
    
/**
 * @brief Handles (prints) packets read from the Mouse
 *
 * @param b Set/unset data reporting
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_set_data_reporting)(bool b);


#endif /* __MOUSE_H */

