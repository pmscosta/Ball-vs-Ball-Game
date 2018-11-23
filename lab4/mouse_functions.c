#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse_functions.h"
#include "kbd_functions.h"

int g_hook_id = 0;

void emptyOutBuffer() {

	unsigned long stat;

	sys_inb(STAT_REG, &stat);

	while (stat & OUT_BUF_FULL) {

		unsigned long waste;

		sys_inb(OUT_BUF, &waste);

		printf("waste: 0x%02x\n", waste);

		sys_inb(STAT_REG, &stat);

	}

}

int mouse_subscribe(void) {

	unsigned int keep_Hook = g_hook_id;  //keeping the original value of hook_id

	if (sys_irqsetpolicy(MS_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &g_hook_id)
			== OK) {

		if (sys_irqenable(&g_hook_id) == OK)

			return BIT(keep_Hook);
	}

	return -1;
}

int mouse_unsubscribe() {

	if (sys_irqrmpolicy(&g_hook_id) == OK) {

		if (sys_irqdisable(&g_hook_id) == OK)

			return 0;
	}

	return 1;
}



int disableDefaultIH() {

	int irq_set_kbd = kbc_subscribe();

	if (irq_set_kbd == -1) //failed to subscribe keyboard
		return 1;

	unsigned long cmd;

	unsigned long stat;

	if (sys_inb(STAT_REG, &stat) != OK)
		return 2;

	if ((stat & IBF) == 0) {

		if (sys_outb(KBC_CMD_REG, KBC_READ_CMD) != OK) //Giving the command to read command byte
			return 3;

		if (sys_inb(OUT_BUF, &cmd) != OK) //Reading command byte which was stored to output buffer
			return 4;
	}

	else
		return 1;

	cmd = cmd | DISABLE_KBD;
	cmd = cmd & DISABLE_DEF_IH;

	if (sys_inb(STAT_REG, &stat) != OK)
		return 5;

	if ((stat & IBF) == 0) {

		if (sys_outb(KBC_CMD_REG, KBC_WRITE_CMD) != OK) //Giving the command to write command byte
			return 6;

		if (sys_outb(OUT_BUF, cmd) != OK) ////Writing the new command  byte, passed as an argument to output buffer
			return 7;

		kbc_unsubscribe();

		return 0;
	}

	else
		return 8;

}

int enableDefaultIH() {

	int irq_set_kbd = kbc_subscribe();

	if (irq_set_kbd == -1) //failed to subscribe keyboard
		return 1;

	unsigned long cmd;

	unsigned long stat;

	if (sys_inb(STAT_REG, &stat) != OK)
		return 2;

	if ((stat & IBF) == 0) {

		if (sys_outb(KBC_CMD_REG, KBC_READ_CMD) != OK) //Giving the command to read command byte
			return 3;

		if (sys_inb(OUT_BUF, &cmd) != OK) //Reading command byte which was stored to output buffer
			return 4;
	}

	else
		return 1;
	cmd = cmd & ~DISABLE_KBD;
	cmd = cmd | ENABLE_DEF_IH;

	if (sys_inb(STAT_REG, &stat) != OK)
		return 5;

	if ((stat & IBF) == 0) {

		if (sys_outb(KBC_CMD_REG, KBC_WRITE_CMD) != OK) //Giving the command to write command byte
			return 6;

		if (sys_outb(OUT_BUF, cmd) != OK) ////Writing the new command  byte, passed as an argument to output buffer
			return 7;

		kbc_unsubscribe();

		return 0;
	}

	else
		return 8;

}

int write_to_mouse(unsigned long cmd) {

	unsigned long stat, response;

	sys_inb(STAT_REG, &stat);

	int attempts = 10;  //the numbers of attempts in case of ERROR response

	int nack_flag = 1;

	while (attempts) {

		if ((stat & IBF) == 0) {

			sys_outb(KBC_CMD_REG, WRITE_MS);  //writes 0xD4

			while (nack_flag && attempts) {

				sys_outb(OUT_BUF, cmd);     //passes the argument

				tickdelay(micros_to_ticks(20000));

				sys_inb(OUT_BUF, &response);

				if (response == NACK) {
					continue;
					attempts--;
				} else if (response == ERROR)
					break;
				else
					return 0;

			}

		}

		tickdelay(micros_to_ticks(DELAY_US));
		attempts--;

	}

	return 1;

}

void print_packet(unsigned long * packet, int length) {
	int i = 0;
	while (i < length) {
		printf("B%d=0x%02x  ", i + 1, packet[i]);
		i++;
	}

	printf("LB=%d  ", (packet[0] & 0x01));
	printf("MB=%d  ", ((packet[0] & 0x04) >> 2));
	printf("RB=%d  ", ((packet[0] & 0x02) >> 1));
	printf("XOV=%d  ", ((packet[0] & 0x40) >> 6));
	printf("YOV=%d  ", ((packet[0] & 0x80) >> 7));

	if ((packet[0] & BIT(4)) == BIT(4)) //negative number
		printf("X= -%d  ", 256 - packet[1]);
	else
		printf("X=%d  ", packet[1]);

	if ((packet[0] & BIT(5)) == BIT(5)) //negative number
		printf("Y= -%d  \n", 256 - packet[2]);
	else
		printf("Y=%d  \n", packet[2]);

}

int get_X_Delta(unsigned long * packet) {

	int delta = 0;

	if ((packet[0] & BIT(4)) == BIT(4))   //if the number is negative
		delta = 0 - (256 - packet[1]); //getting the 2's complement and making it negative
	else
		delta = packet[1];

	return delta;

}

int get_Y_Delta(unsigned long * packet) {

	int delta = 0;

	if ((packet[0] & BIT(5)) == BIT(5))   //if the number is negative
		delta = 0 - (256 - packet[2]); //getting the 2's complement and making it negative
	else
		delta = packet[2];

	return delta;

}

