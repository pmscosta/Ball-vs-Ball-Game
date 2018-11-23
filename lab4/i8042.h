#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_


#define BIT(n) (0x01<<(n))

#define KBC_IRQ	 1

#define MS_IRQ 12

#define KBC_READ_CMD 0x20

#define KBC_WRITE_CMD 0x60

#define WRITE_MS 0xD4

#define STAT_REG 0x64

#define KBC_CMD_REG 0x64

#define OUT_BUF 0x60

#define DELAY_US  20000

#define TWO_BYTE_SCAN 0xE0

#define MSB 0x80

#define DISABLE_KBD BIT(4)

#define ENABLE_INT_KBD BIT(1)

#define ENABLE_INT_MS BIT(0)

#define DISABLE_DEF_IH ~(ENABLE_INT_KBD | ENABLE_INT_MS)

#define ENABLE_DEF_IH (ENABLE_INT_KBD | ENABLE_INT_MS)

#define NACK 0xFE

#define ERROR 0xFC

#define ACK 0xFA

#define PAR_ERR 0x80

#define TO_ERR 0x40

#define OUT_BUF_FULL 0x01

#define AUX_SET 0x20

#define SET_STREAM 0xEA

#define SET_DATA_REPORT 0xF4

#define DISABLE_DATA_REPORT 0xF5

#define SET_REMOTE 0xF0

#define READ_DATA 0xEB

#define MS_REMOTE 0xF0

#define IBF 0x02

#define FIRST_BYTE 0x08

#define TIMER0_IRQ 0

#endif /* _LCOM_I8042_H */
