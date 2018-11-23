#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "video_gr.h"
#include "lmlib.h"
#include "vbe.h"
#include "math.h"
#include "player.h"
/* Private global variables */

static uint16_t *video_mem; /* Process (virtual) address to which VRAM is mapped */
uint16_t * doubleBuffer;
static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static vbe_mode_info_t vbe_info; /* easier access to its components*/
uint16_t * backgroundImage;

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	free(doubleBuffer);
	doubleBuffer = NULL;

	free(backgroundImage);
	backgroundImage = NULL;

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vg_init(unsigned short mode) {

	struct reg86u reg86;
	reg86.u.b.ah = VBE_CALL_FUNCTION;   //0x4F to ah
	reg86.u.b.al = VBE_SET_MODE_FUNCTION;  //0x20 to al
	reg86.u.b.intno = VIDEO_CARD_INTERRUPT_VECTOR; /* BIOS video services */
	reg86.u.w.bx = LINEAR_FRAME_BUFFER | mode;

	if (sys_int86(&reg86) != OK) {
		printf("set_vbe_mode: sys_int_86() failed\n");
		return NULL;
	}

	if(reg86.u.b.ah == 0x01){
		panic("video mode not supported\n");
		return NULL;
	}

	if (reg86.u.w.ax
			== (VBE_CALLED_FUNCTION_SUPPORTED | VBE_FUNCTION_COMPLETED)) {

		if (vbe_get_mode_info(mode, &vbe_info) != 0) //failed to get vbe info
			return NULL;

		bits_per_pixel = vbe_info.BitsPerPixel;
		h_res = vbe_info.XResolution;
		v_res = vbe_info.YResolution;

		int r;

		struct mem_range mr;

		unsigned int vram_base; /* VRAM's physical addresss */

		vram_base = vbe_info.PhysBasePtr;

		unsigned int vram_size; /* VRAM's size, but you can use
		 the frame-buffer size, instead */

		vram_size = h_res * v_res * bits_per_pixel / 8;

		/* Allow memory mapping */

		mr.mr_base = (phys_bytes) vram_base;
		mr.mr_limit = mr.mr_base + vram_size;

		if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
			panic("sys_privctl (ADD_MEM) failed: %d\n", r);

		/* Map memory */

		video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

		if (video_mem == MAP_FAILED)
			panic("couldn't map video memory");

		//initializing Double Buffer

		if ((doubleBuffer = (uint16_t *) malloc(
				h_res * v_res * bits_per_pixel / 8)) == NULL)
			return NULL;

		//initializing backgroundImage buffer

		if ((backgroundImage = (uint16_t *) malloc(
				h_res * v_res * bits_per_pixel / 8)) == NULL)
			return NULL;

		return video_mem;

	}

	return NULL;
}

void vg_colorize_pixel(unsigned short x, unsigned short y, uint16_t color) {

	*(doubleBuffer + (x + y * h_res)) = color;

}

void vg_draw_empty_rect(unsigned short x, unsigned short y,
		unsigned short x_size, unsigned short y_size, unsigned long color) {

	vg_draw_hor_line(x, y, x_size, color);
	vg_draw_hor_line(x, y + y_size, x_size, color);
	vg_draw_vert_line(x, y, y_size, color);
	vg_draw_vert_line(x + x_size, y, y_size, color);

}

void vg_draw_vert_line(unsigned short x, unsigned short yi, unsigned short size,
		unsigned long color) {

	int i;
	for (i = 0; i < size; i++)
		vg_colorize_pixel(x, yi + i, color);

}

void vg_draw_hor_line(unsigned short xi, unsigned short y, unsigned short size,
		unsigned long color) {

	int i;
	for (i = 0; i < size; i++)
		vg_colorize_pixel(xi + i, y, color);

}

/*            BUFFERS FUNCTIONS                */

void vg_swap_video() {
	memcpy(video_mem, doubleBuffer, h_res * v_res * bits_per_pixel / 8);
}

void vg_draw_background() {
	memcpy(doubleBuffer, backgroundImage, h_res * v_res * bits_per_pixel / 8);
}

void vg_clear_screen() {
	memset(doubleBuffer, 0, h_res * v_res * bits_per_pixel / 8);
}

/* ============================================= */

void vg_draw_circle(int x0, int y0, unsigned short radius, unsigned long color) {

	x0 = x0 >= h_res ? (h_res - 1) : x0;

	y0 = y0 >= v_res ? (v_res - 1) : y0;

	x0 = x0 <= 0 ? 0 : x0;

	y0 = y0 <= 0 ? 0 : y0;

	int y, x;

	int black_border_y;
	int black_border_x;

	for (y = -radius, black_border_y = 0; y <= radius; y++, black_border_y++)
		for (x = -radius, black_border_x = 0; x <= radius;
				x++, black_border_x++)
			if (x * x + y * y <= radius * radius)
				vg_colorize_pixel(x0 + x, y0 + y, color);

}

void vg_draw_circle_with_limit(int x0, int y0, unsigned short radius,
		unsigned long color) {

	x0 = x0 >= h_res ? (h_res - 1) : x0;

	y0 = y0 >= v_res ? (v_res - 1) : y0;

	x0 = x0 <= 0 ? 0 : x0;

	y0 = y0 <= 0 ? 0 : y0;

	int y, x;

	int black_border_y;
	int black_border_x;

	for (y = -radius, black_border_y = 0; y <= radius; y++, black_border_y++)
		for (x = -radius, black_border_x = 0; x <= radius;
				x++, black_border_x++) {
			if (x * x + y * y <= radius * radius
					&& x * x + y * y >= (radius * radius) - 125)
				vg_colorize_pixel(x0 + x, y0 + y, COLOR_BLACK);
			else if (x * x + y * y < radius * radius)
				vg_colorize_pixel(x0 + x, y0 + y, color);
		}

}

//====================GET METHODS, FOR EVENTUAL USE OUTSIDE THIS FILE=======================

unsigned getVRes() {
	return v_res;
}

unsigned getHRes() {
	return h_res;
}

uint16_t * getBackgroundImage() {
	return backgroundImage;
}

uint16_t * getDBuf() {
	return doubleBuffer;
}

//===========================================================================================

uint16_t rgb(int r, int g, int b) {

	int red = r * ((1 << vbe_info.RedMaskSize) - 1) / 255;
	int green = g * ((1 << vbe_info.GreenMaskSize) - 1) / 255;
	int blue = b * ((1 << vbe_info.BlueMaskSize) - 1) / 255;

	return (red << vbe_info.RedFieldPosition)
			| (green << vbe_info.GreenFieldPosition)
			| (blue << vbe_info.BlueFieldPosition);

}

void checkMouseBoundaries(int * x, int * y, int height, int width) {

	if ((*x) < 0)
		(*x) = 0;
	if ((*y) < 0)
		(*y) = 0;
	if ((*x) >= h_res)
		(*x) = h_res - 1;
	if ((*y) >= v_res)
		(*y) = v_res - 1;

	if ((*y) + height > v_res)
		(*y) = (v_res - height);
	if ((*x) + width > h_res)
		(*x) = (h_res - width);

	return;

}

