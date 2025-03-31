#ifndef KBC_H
#define KBC_H

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"

#define DELAY_US 20000
extern int32_t kbd_subscription_id;

/** @defgroup kbc kbc
 * @{
 *
 * Functions for using the i8042 KBC
 */

/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @param bit_no Will hold the bit (BIT(hook_id)) to be set in the mask returned upon an interrupt
 * 
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
 * Tries to read a scan code from the kbc, if any error 
 * occurs tries again until READ_ATTEMPT_LIMIT
 */
void (kbd_int_handler)(void);

/**
 * @brief Reads scan codes from the keyboard
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_read_scan_code)(void);

/**
 * @brief Handles (prints) scan codes read from the keyboard
 *
 * @param scan_codes Array that stores scan code
 * 
 * @param timed If set to true resets interrupt_counter 
 */
void (kbd_handle_scan_code)(uint8_t* scan_codes, bool timed);

/**
 * @brief Writes byte in KBC port
 *
 * @param port KBC port to write command into
 * 
 * @param byte Byte to write in port
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_write_byte)(uint8_t port, uint8_t byte);

/**
 * @brief Writes command to KBC_COMMAND_REGISTER
 * 
 * @param command Command to be written in port
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_write_command)(uint8_t command);

/**
 * @brief Writes a command's argument to KBC_ARGUMENT_REGISTER
 * 
 * @param argument Argument to be written
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_write_command_arguments)(uint8_t argument);

/**
 * @brief Reads byte from KBC port
 * 
 * @param port KBC port to read command from
 * 
 * @param byte Will hold the value read from port
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_read_byte)(uint8_t port, uint8_t* byte);

/**
 * @brief Reads a command's return value
 * 
 * Reads the return value of a command from KBC_RETURN_REGISTER to retv
 * 
 * @param retv Will hold the value returned by command
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbc_read_command_return)(uint8_t* retv);


#endif /* __KBC_H */

