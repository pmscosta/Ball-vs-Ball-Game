#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include <minix/com.h>
#include <minix/sysutil.h>
#include "keyboard.h"

static int g_hook_id = KBD_IRQ;

kbd_key * g_key;

void deleteKey(kbd_key * key) {
	if (key == NULL)
		return;
	free(key);
	key = NULL;
}

kbd_key* createKey(unsigned long scancode) {

	kbd_key * key = malloc(sizeof(kbd_key));

	if (key == NULL)
		return NULL;

	/*The generated key will always hold the make code for every single scancode
	 * This way, we only need to keep stored half the macros for knowing which key was pressed/released.
	 * The information about being pressed or released will be kept in the key's member pressed.
	 */

	key->makecode = (scancode & GET_MAKE_CODE);

	if ((scancode & MSB) == MSB)
		key->pressed = 0;
	else
		key->pressed = 1;

	return key;

}

void kbd_int_handler() {

	unsigned long stat;

	static unsigned long two_byte_scan; //if it's a two byte scan, it will hold the first 2 bytes of the scancode

	unsigned long scan;

	if (sys_inb(KBC_STAT_REG, &stat) != 0)
		return;

	if (sys_inb(KBC_OUT_BUF, &scan) != 0)
		return;

	if ((stat & (PARITY_ERROR | TIMEOUT_ERROR)) == 0) {

		//if it is a two_byte scan, must make some room for the next iteration
		if (scan == TWO_BYTE_SCAN) {

			two_byte_scan = TWO_BYTE_SCAN << 8;
			g_key = createKey(DUMMY_KEY);
			return;

		}

		scan = scan | two_byte_scan;

		two_byte_scan = 0;

		g_key = createKey(scan);

		if (scan == KEY_ESC_RELEASE)
			deleteKey(g_key);
	} else{

		g_key = createKey(DUMMY_KEY);
	}

}

int keyboard_subscribe(void) {

	unsigned int keep_Hook = g_hook_id; //keeping the original value of hook_id

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &g_hook_id)
			== OK) {

		if (sys_irqenable(&g_hook_id) == OK) {

			return BIT(keep_Hook);

		}
	}

	return -1;
}

int keyboard_unsubscribe() {

	if (sys_irqrmpolicy(&g_hook_id) == OK) {

		if (sys_irqdisable(&g_hook_id) == OK)

			return 0;
	}

	return 1;
}

