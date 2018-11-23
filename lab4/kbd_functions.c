#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include <minix/com.h>
#include <minix/sysutil.h>
#include "kbd_functions.h"


int g_hook_id_kbd = 0;



int kbc_subscribe(void) {

	unsigned int keep_Hook = g_hook_id_kbd;   //keeping the original value of hook_id


	if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &g_hook_id_kbd) == OK){

		if ( sys_irqenable(& g_hook_id_kbd) == OK)

			return BIT(keep_Hook);
	}

	return -1;
}

int kbc_unsubscribe() {

	if( sys_irqrmpolicy(&g_hook_id_kbd) == OK){

	  if( sys_irqdisable(&g_hook_id_kbd) == OK)

		  return 0;
	}

	return 1;
}
