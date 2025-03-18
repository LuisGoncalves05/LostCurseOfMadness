#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */
#define KBD_IRQ 1 /**< @brief KBD IRQ line */
#define KBD_STATUS_REGISTER 0x64
#define KBD_OUTPUT_BUFFER 0x60

#define TWO_BYTE_SCANCODE 0x0E
#define READ_ATTEMPT_LIMIT 4

#define KBD_PARITY_BIT BIT(7)
#define KBD_TIMEOUT_BIT BIT(6)
#define OUTPUT_BUFFER_FULL BIT(0)

#define READ_COMMAND_BYTE 0x20
#define WRITE_COMMAND_BYTE 0x60

#define CHECK_KBC 0xAA
#define CHECK_KEYBOARD_INTERFACE 0xAB

#define KBD_DISABLE 0xAD
#define KBD_ENABLE 0xAE
/**@}*/

#endif /* _LCOM_I8042_H */
