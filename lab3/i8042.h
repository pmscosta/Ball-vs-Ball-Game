#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_


#define BIT(n) (0x01<<(n))

#define KBC_IRQ	 1

#define KBC_READ 0x20

#define KBC_WRITE 0x60

#define STAT_REG 0x64

#define KBC_CMD_REG 0x64

#define OUT_BUF 0x60

#define DELAY_US  20000

#define TWO_BYTE_SCAN 0xE0

#define MSB 0x80

#define PAR_ERR 0x80

#define TO_ERR 0x40

#define OUT_BUF_FULL 0x01

#define AUX_SET 0x02


#define TIMER0_IRQ 0

#endif /* _LCOM_I8042_H */
