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
#include "video_test.h"
#include "lmlib.h"
#include "vbe.h"
#include "math.h"
#include "sprite.h"

/* Private global variables */

static char *video_mem; /* Process (virtual) address to which VRAM is mapped */
static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

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

	vbe_mode_info_t vbe_info;

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

		//void *video_mem; /* frame-buffer VM address */

		/* Allow memory mapping */

		mr.mr_base = (phys_bytes) vram_base;
		mr.mr_limit = mr.mr_base + vram_size;

		if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
			panic("sys_privctl (ADD_MEM) failed: %d\n", r);

		/* Map memory */

		video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

		if (video_mem == MAP_FAILED)
			panic("couldn't map video memory");

		return video_mem;

	}

	return NULL;
}

int vg_colorize_pixel(unsigned short x, unsigned short y, unsigned long color) {

	if (x >= h_res || y >= v_res)
		return 1;

	*(video_mem + (x + y * h_res) * bits_per_pixel / 8) = color;

	return 0;

}

int vg_draw_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	float middle_x = (h_res - 1) / 2;
	float middle_y = (v_res - 1) / 2;

	float square_center_x = middle_x + x;
	float square_center_y = middle_y + y;

	if ((size & 1) == 0) { //the size is even, thus halving it and adding it with middle_x will cancel out the 0.5 remaining

		int upper_left_x = square_center_x - (size / 2.0);
		int upper_left_y = square_center_y - (size / 2.0);

		int temp_x;
		int temp_y;

		for (temp_x = upper_left_x; temp_x <= upper_left_x + size; temp_x++) {

			for (temp_y = upper_left_y; temp_y <= upper_left_y + size; temp_y++)
				if (vg_colorize_pixel(temp_x, temp_y, color) == 1) //error in changing the color of the pixel
					return 1;

		}

	} else {

		float upper_left_x = square_center_x - (size / 2.0);
		float upper_left_y = square_center_y - (size / 2.0);

		upper_left_x = (int) (upper_left_x + (upper_left_x - 1)) / upper_left_x;
		upper_left_y = (int) (upper_left_y + (upper_left_y - 1)) / upper_left_y;

		int temp_x;
		int temp_y;

		for (temp_x = upper_left_x; temp_x <= upper_left_x + size; temp_x++) {

			for (temp_y = upper_left_y; temp_y <= upper_left_y + size; temp_y++)
				if (vg_colorize_pixel(temp_x, temp_y, color) == 1) //error in changing the color of the pixel
					return 1;

		}

	}

	video_dump_fb();

	return 0;
}

void swap(unsigned short * lhs, unsigned short * rhs) {

	unsigned short temp = *lhs;
	*lhs = *rhs;
	*rhs = temp;
}

int vg_draw_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

	/* Bresenham's Line Algorithm
	 *
	 * As seen here: http://mvscad.blogspot.pt/2012/10/bresenhams-line-for-slope-greater-than-1.html
	 *
	 */

	if (xi > xf) {
		swap(&xi, &xf);
		swap(&yi, &yf);
	}
	long i, j;

	int y_error;

	if (yf < yi)
		y_error = -1;
	else
		y_error = 1;

	unsigned long dx = abs(xf - xi);

	unsigned long dy = abs(yf - yi);

	if (dx > dy) {

		int factor_p = (2 * dy) - dx;

		i = xi;
		j = yi;

		if (vg_colorize_pixel(i, j, color) == 1)
			return 1;

		while (i < xf) {

			i++;

			if (factor_p < 0)
				factor_p = factor_p + 2 * dy;
			else {
				j += y_error;
				factor_p = factor_p + 2 * (dy - dx);
			}

			if (vg_colorize_pixel(i, j, color) == 1)
				return 1;
		}

	} else {

		int factor_p = (2 * dx) - dy;

		i = xi;
		j = yi;

		if (vg_colorize_pixel(i, j, color) == 1)
			return 1;

		while (j != yf) {
			j = j + y_error;

			if (factor_p < 0)
				factor_p = factor_p + 2 * dx;

			else {
				i++;
				factor_p = factor_p + 2 * (dx - dy);
			}

			if (vg_colorize_pixel(i, j, color) == 1)
				return 1;

		}

	}

	video_dump_fb();

	return 0;
}

/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */
char *read_xpm(char *map[], int *wd, int *ht) {
	int width, height, colors;
	char sym[256];
	int col;
	int i, j;
	char *pix, *pixtmp, *tmp, *line;
	char symbol;

	/* read width, height, colors */
	if (sscanf(map[0], "%d %d %d", &width, &height, &colors) != 3) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}
#ifdef DEBUG
	printf("%d %d %d\n", width, height, colors);
#endif
	if (width > h_res || height > v_res || colors > 256) {
		printf("read_xpm: incorrect width, height, colors\n");
		return NULL;
	}

	*wd = width;
	*ht = height;

	for (i = 0; i < 256; i++)
		sym[i] = 0;

	/* read symbols <-> colors */
	for (i = 0; i < colors; i++) {
		if (sscanf(map[i + 1], "%c %d", &symbol, &col) != 2) {
			printf("read_xpm: incorrect symbol, color at line %d\n", i + 1);
			return NULL;
		}
#ifdef DEBUG
		printf("%c %d\n", symbol, col);
#endif
		if (col > 256) {
			printf("read_xpm: incorrect color at line %d\n", i + 1);
			return NULL;
		}
		sym[col] = symbol;
	}

	/* allocate pixmap memory */
	pix = pixtmp = malloc(width * height);

	/* parse each pixmap symbol line */
	for (i = 0; i < height; i++) {
		line = map[colors + 1 + i];
#ifdef DEBUG
		printf("\nparsing %s\n", line);
#endif
		for (j = 0; j < width; j++) {
			tmp = memchr(sym, line[j], 256); /* find color of each symbol */
			if (tmp == NULL) {
				printf("read_xpm: incorrect symbol at line %d, col %d\n",
						colors + i + 1, j);
				return NULL;
			}
			*pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
			printf("%c:%d ", line[j], tmp-sym);
#endif
		}
	}

	return pix;
}

int vg_draw_pixmap(char * pix_map, unsigned short xi, unsigned short yi, int w,
		int h) {

	int i, j;

	for (i = 0; i < w; i++) {

		for (j = 0; j < h; j++) {

			/*
			 * To get the color from the pixel map, the same algorithm applies has in vg_colorize_pixel
			 * Only this time the h_res is the w parameter;
			 */

			unsigned long color = *(pix_map + (i + j * w) * bits_per_pixel / 8);

			vg_colorize_pixel(xi + i, yi + j, color);

		}
	}

	video_dump_fb();

	return 0;

}

int vg_clear_screen() {

	memset(video_mem, 0, h_res * v_res * bits_per_pixel / 8);

	return 0;
}
