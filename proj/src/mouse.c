#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "i8042.h"
#include "video_gr.h"
#include "field.h"

static int g_mouse_hook_id = MOUSE_IRQ;

//==============================INTERRUPT HANDLING CONSTANTS========================

static int byte_counter = 0; //to place the bytes in the correct position, always between 0 and 2

static int firstByteFlag = 0; // 0 = first byte not yet read, 1 = first byte read, reading the following

/**
 * Intertup Handler stores the bytes here,
 * when the packet is ready, copies them to the mouse packet.
 */
static unsigned long temp_packet[3] = { 0, 0, 0 };

//====================================================================================

//======================================MOUSE STRUCT==================================

Mouse * mouse = NULL; //Singleton approach

static Mouse * createMouse() {

	mouse = malloc(sizeof(Mouse));

	if (mouse == NULL)
		return NULL;

	//packet
	mouse->packet[3] = 0;

	//current position
	mouse->x = MOUSE_STARTING_X;
	mouse->y = MOUSE_STARTING_Y;

	//displacement
	mouse->deltaX = 0;
	mouse->deltaY = 0;

	//pressed buttons
	mouse->RButton = 0;
	mouse->LButton = 0;
	mouse->MButton = 0;

	//interrupts
	mouse->beingRead = 1;

	//draw
	mouse->cursor = loadBitmap(IMAGES_PATH "mouse.bmp");
	mouse->ignored_color = IGNORE_PURE_GREEN;

	return mouse;

}

Mouse * getMouse() {

	if (mouse == NULL)
		mouse = createMouse();

	return mouse;

}

void deleteMouse() {
	if (mouse == NULL)
		return;

	deleteBitmap(mouse->cursor);

	free(mouse);
	mouse = NULL;
}

void drawMouse() {

	drawBitmapShape(mouse->cursor, mouse->x, mouse->y, mouse->ignored_color);

}

int get_X_Delta(Mouse * mouse) {

	int delta = 0;

	if ((mouse->packet[0] & MOUSE_X_SIGN) == MOUSE_X_SIGN) //if the number is negative
		delta = 0 - (256 - mouse->packet[1]); //getting the 2's complement and making it negative
	else
		delta = mouse->packet[1];

	return delta;

}

int get_Y_Delta(Mouse * mouse) {

	int delta = 0;

	if ((mouse->packet[0] & MOUSE_Y_SIGN) == MOUSE_Y_SIGN) //if the number is negative
		delta = 0 - (256 - mouse->packet[2]); //getting the 2's complement and making it negative
	else
		delta = mouse->packet[2];

	return delta;

}

void updateMouse() {

	mouse->deltaX = get_X_Delta(mouse);
	mouse->deltaY = get_Y_Delta(mouse);

	mouse->RButton = ((mouse->packet[0] & MOUSE_RIGHT_BUTTON) >> 1);
	mouse->LButton = (mouse->packet[0] & MOUSE_LEFT_BUTTON);
	mouse->MButton = ((mouse->packet[0] & MOUSE_MIDDLE_BUTTON) >> 2);

	mouse->x += (mouse->deltaX * MOUSE_SENSIVITY);
	mouse->y += (-mouse->deltaY * MOUSE_SENSIVITY);

	checkMouseBoundaries(&mouse->x, &mouse->y,
			mouse->cursor->bitmapInfoHeader.height,
			mouse->cursor->bitmapInfoHeader.width);

}

int insideRect(Rectangle * rect) {

	if (mouse->x >= rect->xi && mouse->x <= rect->xf && mouse->y >= rect->yi
			&& mouse->y <= rect->yf)
		return 1;

	return 0;
}

//==============================================================================================

//====================================INTERRUPT=================================================

static void mouse_int_get_byte() {

	unsigned long one_byte;

	if (sys_inb(KBC_OUT_BUF, &one_byte) != OK)
		return;

	if (firstByteFlag == 0) {  //we are expecting the first byte
		if ((one_byte & MOUSE_ALWAYS_ONE_BIT) == 0) //first byte was not found, skip to the next
			return;
		else {  //the first byte was found
			temp_packet[byte_counter] = one_byte;
			firstByteFlag = 1;  //we are now expecting following bytes
			byte_counter = (byte_counter + 1) % NELEMS(temp_packet);
			mouse->beingRead = 1;
		}
	} else {  //we already read the first byte

		temp_packet[byte_counter] = one_byte;
		byte_counter = (byte_counter + 1) % NELEMS(temp_packet);
		mouse->beingRead = 1;

		if (byte_counter == 0) { //if it looped again, we are expecting the first byte again
			firstByteFlag = 0;
			mouse->beingRead = 0; //the packet is now ready
		}

	}

	return;

}

void mouse_int_handler() {

	mouse_int_get_byte(); //mouse_int_handler gets called on every interrupt, thus we must gather the bytes and create a packet

	if (mouse->beingRead == 0 && byte_counter == 0) {

		memcpy(mouse->packet, temp_packet, sizeof(temp_packet));
		updateMouse();
		mouse->beingRead = 1;
		return;
	}
	return;
}

int mouse_subscribe() {

	unsigned int keep_Hook = g_mouse_hook_id; //keeping the original value of hook_id

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&g_mouse_hook_id) == OK) {

		if (sys_irqenable(&g_mouse_hook_id) == OK)

			return BIT(keep_Hook);
	}

	return -1;
}

int mouse_unsubscribe() {

	if (sys_irqrmpolicy(&g_mouse_hook_id) == OK) {

		if (sys_irqdisable(&g_mouse_hook_id) == OK)

			return 0;
	}

	return 1;
}

int writeCommandMouse(unsigned long cmd) {

	unsigned long stat, response;

	int attempts = WRITE_MOUSE_ATTEMPTS; //the numbers of attempts in case of errors

	while (attempts) {

		if (sys_inb(KBC_STAT_REG, &stat) != OK) {
			attempts--;
		} else
			break;
	}

	int nack_flag = 1;

	while (attempts) {

		if ((stat & IBF) == 0) {

			if (sys_outb(KBC_CTRL_REG, KBC_WRITE_BYTE_TO_MOUSE) != OK) { //writes 0xD4
				attempts--;
				continue;
			}
			while (nack_flag && attempts) {

				if (sys_outb(KBC_OUT_BUF, cmd) != OK) { //passes the argument
					attempts--;
					continue;
				}

				tickdelay(micros_to_ticks(DELAY_US)); //waiting a bit to get the response

				if (sys_inb(KBC_OUT_BUF, &response) != OK) {
					attempts--;
					continue;
				}

				if (response == NACK) { //only need to try and write the cmd again
					attempts--;
					continue;

				} else if (response == ERROR) //must try everything again
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

int configureGameMouse() {
	if (writeCommandMouse(MOUSE_SET_STREAM_MODE) == 1)
		return 1;

	if (writeCommandMouse(MOUSE_ENABLE_DATA_REPORT) == 1)
		return 1;

	return 0;
}

//==============================================================================================

