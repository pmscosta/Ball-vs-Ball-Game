#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#include "timer_functions.h"

int g_counter = 0;

int hook_id_timer = 1;

int timer_subscribe_int(void) {

	 int keep_Hook = hook_id_timer;   //preservando o valor original de g_hook_id para depois comparar


	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) == OK){

		if ( sys_irqenable(& hook_id_timer) == OK)

			return BIT(keep_Hook);
	}

	return -1;
}

int timer_unsubscribe_int() {

	if( sys_irqrmpolicy(&hook_id_timer) == OK){

	  if( sys_irqdisable(&hook_id_timer) == OK)

		  return 0;
	}

	return 1;
}

void timer_int_handler() {
	g_counter++;

}
