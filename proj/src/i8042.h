/**
 * @file i8042.h
 * @author Pedro Costa, João Fidalgo
 * @date 2 Jan 2018
 * @brief Constants for the KBC.
 *
 */

#ifndef _I8042_H
#define _I8042_H


/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the LKBC
 */

#define BIT(n) (0x01<<(n))

#define DELAY_US  20000

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

//===============IRQ LINES==========================

#define KBD_IRQ	 1

#define MOUSE_IRQ 12

//================KBC PORTS=========================

#define KBC_OUT_BUF 0x60

#define KBC_IN_BUF 0x60

#define KBC_STAT_REG 0x64

#define KBC_CTRL_REG 0x64


//================KBC COMMANDS================

#define KBC_READ_COMMAND_BYTE 0x20

#define KBC_WRITE_COMMAND_BYTE 0x60

#define KBC_WRITE_BYTE_TO_MOUSE 0xD4

#define KBC_MAKE_AND_BREAK 0xFA


//===================KBC COMMAND BYTE=====================

#define ENABLE_INT_KBD BIT(1)

#define ENABLE_INT_MS BIT(0)

#define DISABLE_KBD BIT(4)

#define DISABLE_MOUSE BIT(5)

#define DISABLE_DEF_IH ~(ENABLE_INT_KBD | ENABLE_INT_MS)

#define ENABLE_DEF_IH (ENABLE_INT_KBD | ENABLE_INT_MS)


//=====================KBC RESPONSES========================

#define NACK 0xFE

#define ERROR 0xFC

#define ACK 0xFA

//=======================KBC STATUS REGISTER=================

#define OBF 0x01

#define IBF 0x02

#define PARITY_ERROR 0x80

#define TIMEOUT_ERROR 0x40

#define AUX_SET 0x20



//=======================KBD MACROS==========================

#define TWO_BYTE_SCAN 0xE0

#define MSB 0x80

#define DISABLE_KBD BIT(4)

#define GET_MAKE_CODE 0xFF7F

#define TEST_TWO_BYTE_SCAN 0xFF00

#define GET_SCAN_POSITION 0x007F


//========================MOUSE MACROS=====================


#define MOUSE_RESET						0xFF

#define MOUSE_RESEND					0xFE

#define MOUSE_SET_DEFAULT				0xF6

#define MOUSE_DISABLE_STREAM_MODE		0xF5

#define MOUSE_ENABLE_DATA_REPORT  		0xF4

#define MOUSE_SET_SAMPLE_RATE			0xF3

#define MOUSE_SET_REMOTE_MODE			0xF0

#define MOUSE_READ_DATA					0xEB

#define MOUSE_SET_STREAM_MODE			0xEA

#define WRITE_MOUSE_ATTEMPTS			10


//------>DATA PACKET

#define MOUSE_Y_OVF 					0x80
#define MOUSE_X_OVF						0x40
#define MOUSE_Y_SIGN                    0x20
#define MOUSE_X_SIGN					0x10
#define MOUSE_ALWAYS_ONE_BIT			0x08
#define MOUSE_MIDDLE_BUTTON				0x04
#define MOUSE_RIGHT_BUTTON				0x02
#define MOUSE_LEFT_BUTTON				0x01

/**@}*/

#endif

