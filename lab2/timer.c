#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "timer.h"
#include <minix/com.h>

unsigned int g_hook_id = 0x00;

int g_counter = 0;


int timer_set_frequency(unsigned char timer, unsigned long freq) {

	unsigned char newConf;
	if (freq == 0 || freq > TIMER_FREQ) //a nova freq nao pode ser 0 ou superior a freq nativa
		return 1;

	if (timer >= 0 && timer <=2){

		timer_get_conf(timer, &newConf);  //acedendo a configuracao inicial para preservar os 4 lsb's

		newConf = 0x0F & newConf; //preservando os primeiros 4 bits

		newConf = TIMER_ADDR(timer) | TIMER_LSB_MSB | newConf;

		newConf = (unsigned long) newConf;

		sys_outb(TIMER_CTRL, newConf);

		sys_outb(TIMER_ADDR(timer), TIMER_FREQ/freq);

		sys_outb(TIMER_ADDR(timer), (TIMER_FREQ/freq >> 8 ));

		return 0;
	}

	return 1;
}

int timer_subscribe_int(void) {

	unsigned int keep_Hook = g_hook_id;   //preservando o valor original de g_hook_id para depois comparar


	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &g_hook_id) == OK){

		if ( sys_irqenable(& g_hook_id) == OK)

			return BIT(keep_Hook);
	}

	return -1;
}

int timer_unsubscribe_int() {

	if( sys_irqrmpolicy(&g_hook_id) == OK){

	  if( sys_irqdisable(&g_hook_id) == OK)

		  return 0;
	}

	return 1;
}

void timer_int_handler() {
	g_counter++;

}

int timer_get_conf(unsigned char timer, unsigned char *st) {
	
	unsigned long reader;

	if (timer >= 0 && timer <=2){

		reader = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); //selecionando o read back command e o timer, obter o status

		sys_outb(TIMER_CTRL, reader);

		sys_inb(TIMER_ADDR(timer), &reader);

		*st = (unsigned char) reader;

		return 0;
	}



	return 1;
}

int timer_display_conf(unsigned char conf) {
	

	//====OUTPUT PIN

	printf("Output pin: %d\n", (BIT(7) & conf)>>7);

	//=====NULL COUNTER

	if(BIT(6) & conf)                  //if bit 6 is 1...
		printf("Null count\n");
	else
		printf("Count available for reading\n");


	//=====Type of Access

	unsigned char maskType = BIT(5) | BIT(4);

	switch((maskType & conf) >> 4){                       //testing bits 4 and 5 ...
	case 1:
			printf("Type of Access: LSB\n");
		break;
	case 2:
			printf("Type of Access: MSB\n");
		break;
	case 3:
			printf("Type of Access: LSB followed by MSB\n");
		break;
	}


	//====Operating Mode

	unsigned char maskOper = BIT(3) | BIT(2) | BIT (1);
	maskOper = (maskOper & conf) >> 1;

	if ( maskOper < 6){
	printf("Operating Mode: %d\n", maskOper);
	}

	//dont care bits

	else if (maskOper == 6)
		printf("Operating Mode: 2\n");
	else
		printf("Operating Mode: 3\n");




	//====Counting Mode

	unsigned char maskCount = BIT(0);

	if(maskCount & conf)           //testing if bit 0 is 1...
		printf("Counting mode: BCD\n");
	else
		printf("Counting mode: Binary\n");

	return 1;
}

int timer_test_time_base(unsigned long freq) {
	
	if( timer_set_frequency(0, freq) == 0)
		return 0;

	return 1;
}


int timer_test_int(unsigned long time) {
	

	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	int irq_set = timer_subscribe_int();

	if (irq_set == -1) //failed to subscribe timer0
		return 1;


	while( g_counter/60 <= time){   /*given that interrupt counter
									increases 60 times every sec*/


		if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {

			printf("driver_receive failed with: %d", r);

		 	continue;

		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
		  	  case HARDWARE:

		  		  /* hardware interrupt notification */

		  		  if (msg.NOTIFY_ARG & irq_set) {

		  			 if (g_counter % 60 == 0) //if a second passes...

		  			  printf("Seconds Elapsed: %d \n", g_counter/60 );

		  			 timer_int_handler();
		  		  }
		  		  break;
		  	  default:
		  		  break;
		  		  /* no other notifications expected: do nothing */
			}
		}
		else {
			/* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	timer_unsubscribe_int();

	return 0;
}

int timer_test_config(unsigned char timer) {
	
	unsigned char timerConf;
	int error;

	if (timer >= 0 && timer <= 2){

		error = timer_get_conf(timer, &timerConf);

		if (error == 1)
			return 1;

		error = timer_display_conf(timerConf);

		if(error == 1)
			return 1;

		return 0;

	}

	return 1;
}
