#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/driver.h>
#include "i8042.h"
#include <minix/com.h>
#include "test4.h"
#include "mouse_functions.h"
#include "timer_functions.h"

/***********************************STATE MACHINE BAREBONES***************************************/

typedef enum {
	INIT, DRAW
} state_t;

typedef enum {
	RDOWN, RUP, MOVE
} ev_type_t;

typedef struct {

	ev_type_t type;

	unsigned long * evt_packet;

} event_t;

/**************************************************************************************************/

int handle_event(event_t *evt, short length);

int mouse_test_packet(unsigned short cnt) {
	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	if (write_to_mouse(SET_STREAM) != 0)
		return 1;

	if (write_to_mouse(SET_DATA_REPORT) != 0)
		return 1;

	int irq_set = mouse_subscribe();

	if (irq_set == -1) //failed to subscribe mouse
		return 1;

	unsigned long packet[3];

	int byte_counter = 0;

	int first_byte_flag = 1;

	unsigned long one_byte = 0;

	while (cnt) {

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

					sys_inb(OUT_BUF, &one_byte);

					if (first_byte_flag) {
						if ((one_byte & FIRST_BYTE) == 0)
							break;
						else {
							packet[byte_counter] = one_byte;
							first_byte_flag = 0;
							++byte_counter;
						}
					} else {
						if (byte_counter == 2) {
							packet[byte_counter] = one_byte;
							cnt--;
							print_packet(packet, 3);
							byte_counter = 0;
						} else {
							packet[byte_counter] = one_byte;
							++byte_counter;
						}
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

	mouse_unsubscribe();

	if (write_to_mouse(DISABLE_DATA_REPORT) != 0)
		return 1;

	return 0;
}

int mouse_test_async(unsigned short idle_time) {
	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	int time_elapsed = 0;

	if (write_to_mouse(SET_STREAM) != 0)
		return 1;

	if (write_to_mouse(SET_DATA_REPORT) != 0)
		return 1;

	int irq_set_mouse = mouse_subscribe();

	if (irq_set_mouse == -1) //failed to subscribe mouse
		return 1;

	int irq_set_timer = timer_subscribe_int();

	if (irq_set_timer == -1) //failed to subscribe timer
		return 1;

	unsigned long packet[3];

	int byte_counter = 0;

	int first_byte_flag = 1;

	unsigned long one_byte;

	while (time_elapsed < idle_time) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {

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

				//MOUSE INTERRUPT NOTIFICATION

				if (msg.NOTIFY_ARG & irq_set_mouse) {

					sys_inb(OUT_BUF, &one_byte);

					if (first_byte_flag) {
						if ((one_byte & FIRST_BYTE) == 0)
							break;
						else {
							packet[byte_counter] = one_byte;
							first_byte_flag = 0;
							byte_counter++;
						}
					} else {
						if (byte_counter == 2) {
							packet[byte_counter] = one_byte;
							print_packet(packet, 3);
							byte_counter = 0;
							time_elapsed = 0;
						} else {
							packet[byte_counter] = one_byte;
							byte_counter++;
						}
					}
				}

				//====================================
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
	mouse_unsubscribe();

	timer_unsubscribe_int();

	if (write_to_mouse(DISABLE_DATA_REPORT) != 0)
		return 1;

	return 0;
}

int mouse_test_remote(unsigned long period, unsigned short cnt) {

	int erro = disableDefaultIH();

	if (erro != 0) {
		return 1;
	}

	if (write_to_mouse(SET_DATA_REPORT) != 0) //i dont know why but this is needed
		return 1;

	if (write_to_mouse(DISABLE_DATA_REPORT) != 0)
		return 1;

	if (write_to_mouse(SET_REMOTE) != 0)
		return 1;

	unsigned long stat;

	unsigned long packet[3];

	unsigned long one_byte = 0;

	while (cnt) {

		if (write_to_mouse(READ_DATA) != 0)
			continue;

		if (sys_inb(STAT_REG, &stat) != OK)
			return 1;

		if ((stat & AUX_SET) == AUX_SET && (stat & OUT_BUF_FULL) == 1
				&& (stat & (PAR_ERR | TO_ERR)) == 0) {

			sys_inb(OUT_BUF, &one_byte);
			packet[0] = one_byte;
		}

		if ((stat & AUX_SET) == AUX_SET && (stat & OUT_BUF_FULL) == 1
				&& (stat & (PAR_ERR | TO_ERR)) == 0) {
			sys_inb(OUT_BUF, &one_byte);
			packet[1] = one_byte;
		}

		if ((stat & AUX_SET) == AUX_SET && (stat & OUT_BUF_FULL) == 1
				&& (stat & (PAR_ERR | TO_ERR)) == 0) {
			sys_inb(OUT_BUF, &one_byte);
			packet[2] = one_byte;
		}

		print_packet(packet, 3);

		cnt--;

		tickdelay(micros_to_ticks(period * 1000));

		emptyOutBuffer();

	}

	if (enableDefaultIH() != 0)
		return 1;

	if (write_to_mouse(SET_STREAM) != 0)
		return 1;

	if (write_to_mouse(DISABLE_DATA_REPORT) != 0)
		return 1;

	return 0;
}

int mouse_test_gesture(short length) {
	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	if (write_to_mouse(SET_STREAM) != 0)
		return 1;

	if (write_to_mouse(SET_DATA_REPORT) != 0)
		return 1;

	int irq_set = mouse_subscribe();

	if (irq_set == -1) //failed to subscribe mouse
		return 1;

	unsigned long packet[3];

	int byte_counter = 0;

	int first_byte_flag = 1;

	unsigned long one_byte = 0;

	int stoping = 1;

	while (1) {

		if (stoping == 0)
			break;

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

					sys_inb(OUT_BUF, &one_byte);

					if (first_byte_flag) {
						if ((one_byte & FIRST_BYTE) == 0)
							break;
						else {
							packet[byte_counter] = one_byte;
							first_byte_flag = 0;
							++byte_counter;
						}
					} else {
						if (byte_counter == 2) {

							/*
							 * creating the packet,
							 * thus a new mouse event will be created
							 */
							packet[byte_counter] = one_byte;
							print_packet(packet, 3);
							byte_counter = 0;
							event_t evt;
							evt.evt_packet = packet;  //assigning the new packet
							stoping = handle_event(&evt, length);

						} else {
							packet[byte_counter] = one_byte;
							++byte_counter;
						}
					}

				}
				break;
			default:
				break;
				/* no other notifications expected: do nothing */
			}
		} else {
			break;
			/* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}

	}

	mouse_unsubscribe();

	if (write_to_mouse(DISABLE_DATA_REPORT) != 0)
		return 1;

	printf("Line was recognized! Exiting...\n");

	return 0;
}

int check_upwards_line(event_t *evt, short length) {

	static state_t st = INIT;        //initial state
	static int horizontal_stat = 0;  //storing the horizontal movements

	switch (st) {

	case INIT:
		if (evt->type == RDOWN)
			st = DRAW;
		break;
	case DRAW:

		if (evt->type == MOVE) {

			if (length > 0) {

				/*
				 * if we are doing an upwards movement,
				 * the y delta can't dip below 1 unit (in relation to the previous y position)
				 */
				if (get_Y_Delta(evt->evt_packet) < 1) {
					horizontal_stat = 0;
					st = INIT;
					break;
				}

				if (get_X_Delta(evt->evt_packet) < 0) { //movement was made in the opposite x direction
					horizontal_stat = 0;
					st = INIT;
					break;
				} else
					horizontal_stat += get_X_Delta(evt->evt_packet);
			}
			/*
			 * if we are doing a downwards movement,
			 * the y delta can't be positive (in relation to the previous y position)
			 */
			else {

				if (get_Y_Delta(evt->evt_packet) > -1) { //means that the y decreased below the tolerance, we want a upwards motion
					horizontal_stat = 0;
					st = INIT;
					break;
				}

				if (get_X_Delta(evt->evt_packet) > 0) { //movement was made in the opposite x direction
					horizontal_stat = 0;
					st = INIT;
					break;
				} else
					horizontal_stat += get_X_Delta(evt->evt_packet);
			}

			if (abs(horizontal_stat) > abs(length))  //if the accumulated horizontal distance is enough, finished
				return 0;

		}

		else if (evt->type == RUP) {
			horizontal_stat = 0;
			st = INIT;
		}
		break;
	default:
		break;
	}

	return 1;

}

int handle_event(event_t *evt, short length) {
	if ((evt->evt_packet[0] & BIT(1)) == BIT(1)) {  //right button was pressed
		evt->type = RDOWN;
		if (check_upwards_line(evt, length) == 0)
			return 0;
	} else {                                        //right button was released
		evt->type = RUP;
		if (check_upwards_line(evt, length) == 0)
			return 0;
	}

	if (get_X_Delta(evt->evt_packet) != 0
			|| get_X_Delta(evt->evt_packet) != 0) { //meaning there was some sort of movement
		evt->type = MOVE;
		if (check_upwards_line(evt, length) == 0)
			return 0;
	}

	return 1;

}
