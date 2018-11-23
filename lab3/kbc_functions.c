#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
#include <minix/com.h>
#include <minix/sysutil.h>
#include "kbc_functions.h"




#ifdef LAB3
int sys_inb_cnt(port_t port, unsigned long *byte){
	g_sys_cnt++;

	return sys_inb(port, byte);

}
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif



int g_hook_id = 0;
int g_sys_cnt = 0;
int g_scancode = 0;




int kbc_subscribe(void) {

	unsigned int keep_Hook = g_hook_id;   //keeping the original value of hook_id


	if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &g_hook_id) == OK){

		if ( sys_irqenable(& g_hook_id) == OK)

			return BIT(keep_Hook);
	}

	return -1;
}

int kbc_unsubscribe() {

	if( sys_irqrmpolicy(&g_hook_id) == OK){

	  if( sys_irqdisable(&g_hook_id) == OK)

		  return 0;
	}

	return 1;
}


void kbc_int_handler(){
	unsigned long stat;

	unsigned long data;

	sys_inb_cnt(STAT_REG, &stat);

	sys_inb_cnt(OUT_BUF, &data); /* assuming it returns OK */

	if ((stat & (PAR_ERR | TO_ERR)) == 0) {
		g_scancode = (int) data;
	}
	else
		g_scancode = 0;    //scancode is 0 when data is corrupted
}

void kbc_read_code_poll() {

	unsigned long stat;

	unsigned long data;

	int counter = 10; //the maximum number of failed attempts to read will be 10

	while (counter) {
		sys_inb_cnt(STAT_REG, &stat); /* assuming it returns OK */

		/* loop while 8042 output buffer is empty */

		if ((stat & OUT_BUF_FULL) && ((stat & AUX_SET) == 0)) {

			sys_inb_cnt(OUT_BUF, &data);

			if ((stat & (PAR_ERR | TO_ERR)) == 0) {

				g_scancode = (int) data;

				break;
			}

			else{
				g_scancode = 0; //data was corrupted
				break;
			}

		}

		tickdelay(micros_to_ticks(DELAY_US));
		counter--;
	}

	if (counter == 0)   //if the number of max attempts reached the end...
		g_scancode = -1;  //scancode is -1 when the output buffer never got full
}




void print_scancode(int  * scan_flag){

	 if (g_scancode == TWO_BYTE_SCAN){          //printing two bytes scan

		*scan_flag = 1;

		}

	else if(*scan_flag == 1)//printing one byte scan
	{

		if ((g_scancode & MSB) == MSB){

			 printf("Breakcode: 0x%02x", TWO_BYTE_SCAN);

			 printf(" 0x%02x\n", g_scancode);

		 }
		 else {

			 printf("Makecode: 0x%02x", TWO_BYTE_SCAN);
			 printf(" 0x%02x\n", g_scancode);

		 }
			*scan_flag = 0;
	}
	  else{
		if((g_scancode & MSB) == MSB)
			printf("Breakcode: 0x%02x\n", g_scancode);
		else
			printf("Makecode: 0x%02x\n", g_scancode);
		  }

}




