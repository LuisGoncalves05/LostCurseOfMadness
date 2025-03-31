#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */

#define KBD_IRQ 1

#define KBC_STATUS_REGISTER 0x64
#define KBC_COMMAND_REGISTER 0x64
#define KBC_OUTPUT_BUFFER 0x60
#define KBC_ARGUMENT_REGISTER 0x60
#define KBC_RETURN_REGISTER 0x60

#define TWO_BYTE_SCANCODE 0xE0
#define ESC_BREAK_CODE 0x81
#define BREAK BIT(7)

#define KBC_PARITY_BIT BIT(7)
#define KBC_TIMEOUT_BIT BIT(6)
#define KBC_OUTPUT_BUFFER_FULL BIT(0)
#define KBC_INPUT_BUFFER_FULL BIT(1)

#define KBD_INTERRUPTS_ON BIT(0)

#define KBC_READ_COMMAND_BYTE 0x20
#define KBC_WRITE_COMMAND_BYTE 0x60

#define READ_ATTEMPT_LIMIT 20

/**@}*/

#endif /* _LCOM_I8042_H */
