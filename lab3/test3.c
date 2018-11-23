#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8042.h"
#include <minix/com.h>
#include "kbc_functions.h"
#include "timer_functions.h"


extern int scan_asm;


void kbc_IH_asm();


int kbd_test_scan(unsigned short assembly) {
	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	int irq_set = kbc_subscribe();

	if (irq_set == -1) //failed to subscribe keyboard
		return 1;


	int scan_flag = 0;

	while (g_scancode != 0x81) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {

			printf("driver_receive failed with: %d", r);

			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				/* hardware interrupt notification */

				if (msg.NOTIFY_ARG & irq_set) {
					if (assembly == 0) {
						kbc_int_handler();

						if (g_scancode != 0) {      //success on reading scancode, it wasn't corrupted

							print_scancode(&scan_flag);

						}



					} else {

						kbc_IH_asm();

						g_scancode = scan_asm;

						print_scancode(&scan_flag);



					}
				}
				break;
			default:
				break;
				/* no other notifications expected: do nothing */
			}
		} else {
			/* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}

	}

	if (assembly == 0)                           //only printing the sys_inb calls if the used IH was in C
#ifdef LAB3
		printf("sys_inb calls: %d\n", g_sys_cnt);
#endif

	kbc_unsubscribe();

	return 0;
}

int kbd_test_poll() {

	int scan_flag = 0;

	unsigned long cmd;


	while(g_scancode != 0x81){

		kbc_read_code_poll();

		if (g_scancode != -1 && g_scancode != 0)  //testing if data was not corrupted or if the out_buf never got full
			print_scancode(& scan_flag);



	}

#ifdef LAB3
	printf("sys_inb calls: %d\n", g_sys_cnt);
#endif



	//Enabling MINIX IH


	sys_outb(KBC_CMD_REG, KBC_READ); //Giving the command to read command byte

	sys_inb(OUT_BUF, &cmd);          //Reading command byte which was stored to output buffer

	cmd = cmd | 0x01;                //Setting least significant bit to 1 to enable

	sys_outb(KBC_CMD_REG, KBC_WRITE);   //Giving the command to write command byte

	sys_outb(OUT_BUF, cmd);         ////Writing the new command  byte, passed as an argument to output buffer


	//==================


	return 0;
}



int kbd_test_timed_scan(unsigned short n) {

			int r;   //to store driver_receiver return value

			int ipc_status;

			message msg;

			int irq_set_kbd = kbc_subscribe();

			if (irq_set_kbd == -1) //failed to subscribe keyboard
				return 1;

			int irq_set_timer = timer_subscribe_int();

			if (irq_set_timer == -1) //failed to subscribe timer
				return 1;

			int scan_flag = 0;

			unsigned short time_elapsed = 0;


			while(g_scancode != 0x81 && time_elapsed < n){

				if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {

					printf("driver_receive failed with: %d", r);

				 	continue;

				}
				if (is_ipc_notify(ipc_status)) {
					/* received notification */
					switch (_ENDPOINT_P(msg.m_source)) {
				  	  case HARDWARE:

				  		  /* hardware interrupt notification */


				  		  //TIMER INTERRUPT NOTIFICATION

				  		  if (msg.NOTIFY_ARG & irq_set_timer) {

				  			  if (g_counter % 60 == 0) //if a second passes...

				  				  time_elapsed++;

				  			 timer_int_handler();

				  			}

				  		  //============================


				  		  //KEYBOARD INTERRUPT NOTIFICATION

				  		  if (msg.NOTIFY_ARG & irq_set_kbd) {

				  			  kbc_int_handler();  //using C-written IH

				  			  if (g_scancode != 0){          //success on reading scancode

				  				  print_scancode(& scan_flag);

				  				  time_elapsed = 0;      //resetting the time elapsed since the last keystroke

				  		  }

				  		//==================================


				  			  break;
				  	  default:
				  		  break;
				  		  /* no other notifications expected: do nothing */
				  		  }
					}
				}
				else {
					/* received a standard message, not a notification */
					/* no standard messages expected: do nothing */
				}

			}


			kbc_unsubscribe();

			timer_unsubscribe_int();

			return 0;

}
