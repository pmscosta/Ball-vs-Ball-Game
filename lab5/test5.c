#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <sys/mman.h>
#include "test5.h"
#include "vbe.h"
#include "video_gr.h"
#include "timer_functions.h"
#include "kbd_functions.h"
#include "video_test.h"
#include "sprite.h"
#include "i8042.h"

int video_test_init(unsigned short mode, unsigned short delay) {

	if (vg_init(mode) == NULL) {
		vg_exit();
		printf("Nao foi possivel trocar de modo\n");
		return 0;
	}

	int irq_set = timer_subscribe_int();

	if (irq_set == -1) { //failed to subscribe timer0
		printf("Failed to subscribe the timer.\n");
		return 0;
	}

	int time_elapsed = 0;

	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	while (time_elapsed < delay) {

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

					if (g_counter % 60 == 0) //if a second passes...

						time_elapsed++;

					timer_int_handler();
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

	timer_unsubscribe_int();

	vg_exit();

	return 1;

}

int video_test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	if (vg_init(VBE_GRAPHIC_MODE_1024_768_256) == NULL) {
		vg_exit();
		printf("Nao foi possivel trocar de modo\n");
		return 1;
	}

	if (vg_draw_square(x, y, size, color) != 0) {
		vg_exit();
		printf("Nao foi possivel desenhar o quadrado\n");
		return 1;
	}

	//==================================== KBD INTERRUPT ===================================

	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	int irq_set = kbc_subscribe();

	if (irq_set == -1) //failed to subscribe keyboard
		return 1;

	while (g_scancode != ESC_KEY) {

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

					kbc_int_handler();

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

	kbc_unsubscribe();

	//=============================END OF KBD INTERRUPTS=======================================

	vg_exit();

	return 0;

}

int video_test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

	if (vg_init(VBE_GRAPHIC_MODE_1024_768_256) == NULL) {
		vg_exit();
		printf("Nao foi possivel trocar de modo\n");
		return 1;
	}

	if (vg_draw_line(xi, yi, xf, yf, color) != 0) {
		vg_exit();
		printf("Nao foi possivel desenhar o quadrado\n");
		return 1;
	}

	//==================================== KBD INTERRUPT ===================================

	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	int irq_set = kbc_subscribe();

	if (irq_set == -1) //failed to subscribe keyboard
		return 1;

	while (g_scancode != ESC_KEY) {

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

					kbc_int_handler();

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

	kbc_unsubscribe();

	//=============================END OF KBD INTERRUPTS=======================================

	vg_exit();

	return 0;
}

int test_xpm(char *xpm[], unsigned short xi, unsigned short yi) {
	char * pix_map;

	int w, h;

	if (vg_init(VBE_GRAPHIC_MODE_1024_768_256) == NULL) {
		vg_exit();
		printf("Nao foi possivel trocar de modo\n");
		return 1;
	}

	pix_map = read_xpm(xpm, &w, &h);

	if (pix_map == NULL) {
		vg_exit();
		printf("Nao foi possivel ler o XPM.\n");
		return 1;

	}

	if (vg_draw_pixmap(pix_map, xi, yi, w, h) != 0) {
		vg_exit();
		printf("%d %d\n", w, h);
		printf("Nao foi possivel desenhar o XPM.\n");
		return 1;
	}

	//==================================== KBD INTERRUPT ===================================

	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	int irq_set = kbc_subscribe();

	if (irq_set == -1) //failed to subscribe keyboard
		return 1;

	while (g_scancode != ESC_KEY) {

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

					kbc_int_handler();

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

	kbc_unsubscribe();

	//=============================END OF KBD INTERRUPTS=======================================

	vg_exit();

	return 0;

}

int test_move(char *xpm[], unsigned short xi, unsigned short yi,
		unsigned short xf, unsigned short yf, short s, unsigned short f) {

	if (f > 60) {
		printf("Fps superior ao suportado pelo timer, em default. %d\n", f);
		return 1;
	}

	if (vg_init(VBE_GRAPHIC_MODE_1024_768_256) == NULL) {
		vg_exit();
		printf("Nao foi possivel trocar de modo\n");
		return 1;
	}

	Sprite * sp = create_sprite(xpm, xi, yi, s, s);

	if (xi == xf) //only vertical movement
		sp->xspeed = 0;
	if (yi == yf) //only horizontal movement
		sp->yspeed = 0;

	/*
	 * If frames meaning is 0, we must take speed as the movement of pixels between each frame
	 * Otherwise it will be the frames required for a displacement of a pixel
	 */

	int framesMeaning = 0;

	if (sp->xspeed < 0) {
		framesMeaning = 1;
		sp->xspeed = 1;
	}
	if (sp->yspeed < 0) {
		framesMeaning = 1;
		sp->yspeed = 1;
	}

	int fps = 60 / f;

	//==================================== KBD AND TIMER INTERRUPT ===================================

	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	int irq_kbd_set = kbc_subscribe();

	int irq_timer_set = timer_subscribe_int();

	if (irq_kbd_set == -1) //failed to subscribe keyboard
		return 1;
	if (irq_timer_set == -1) //failed to subscribe timer
		return 1;

	draw_sprite(sp);

	while (g_scancode != ESC_KEY) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {

			printf("driver_receive failed with: %d", r);

			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				/* hardware interrupt notification */

				/*
				 * TIMER INTERRUPT
				 */

				if (msg.NOTIFY_ARG & irq_timer_set) {

					if (sp->x <= xf && sp->y <= yf) {

						if (framesMeaning == 0) {

							if ((g_counter % fps) == 0) {

								vg_clear_screen(0);
								animate_sprite(sp);

							}
						} else {

							if ((g_counter % s) == 0) {

								vg_clear_screen(0);
								animate_sprite(sp);

							}

						}
					}

					timer_int_handler();

				}

				/*
				 * KBD INTERRUPT
				 */
				if (msg.NOTIFY_ARG & irq_kbd_set) {

					kbc_int_handler();

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

	kbc_unsubscribe();

	timer_unsubscribe_int();

	//===========================END OF KBD AND TIMER INTERRUPTS=====================================
	vg_exit();

	destroy_sprite(sp);

	return 0;

}

int test_controller() {

	vbe_info_block_t vib_p;

	if (vbe_get_info_block(&vib_p) == 1) {

		return 1;

	}

	else {

		int vbe_signature = (int) vib_p.VBEVersion;

		uint16_t highVersion = ((vbe_signature & 0xFF00) >> 8);

		int decHigh = ((highVersion & 0xF0) >> 4) * 10 + (highVersion & 0x0F);

		uint16_t lowVersion = ((vbe_signature & 0x00FF));

		int decLow = ((lowVersion & 0xF0) >> 4) * 10 + (lowVersion & 0x0F);

		printf("%d.%d\n", decHigh, decLow);

		uint16_t * count_modes = (uint16_t *) (vib_p.Reserved);

		if (*count_modes == LIST_MODE_TERMINATED) {
			printf("Only stub Implementation.\n");
			return 1;

		}

		int first = 1;

		while (*count_modes != LIST_MODE_TERMINATED) {

			if (first == 1)
				printf("0x%X:", *count_modes);

			first = 0;
			printf("0x%X:", *count_modes);

			count_modes++;

		}

		printf("\n%d", vib_p.TotalMemory * 64);

	}

	return 0;
}

