#ifndef __LAB5_C
#define __LAB5_C

#include "test5.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>
#include <string.h>

static int proc_args(int argc, char **argv);
static unsigned short parse_ushort(char *str, int base);
static unsigned long parse_ulong(char *str, int base);
static short parse_short(char *str, int base);
static void print_usage(char **argv);
void sef_startup();
char ** getPixMap(char * name);

int main(int argc, char **argv) {
	/* DO NOT FORGET TO initialize service */
	sef_startup();

	if (argc == 1) { /* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	} else
		return proc_args(argc, argv);
}

static void print_usage(char **argv) {
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"init mode delay\"\n"
			"\t service run %s -args \"square x y size color\"\n"
			"\t service run %s -args \"line xi yi xf yf color\"\n"
			"\t service run %s -args \"xpm xpm xi yi\"\n"
			"\t service run %s -args \"move xpm xi yi xf yf speed fps\"\n"
			"\t service run %s -args \"controller", argv[0], argv[0], argv[0],
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv) {
	unsigned short mode, delay, x, y, size, xi, yi, xf, yf, fps;
	unsigned long color;
	char ** xpm;
	short speed;
	if (strncmp(argv[1], "init", strlen("init")) == 0) {
		if (argc != 4) {
			printf(
					"video: wrong no. of arguments for video_test_init(mode, delay)\n");
			return 1;
		}
		mode = parse_ushort(argv[2], 16);
		if (mode == USHRT_MAX)
			return 1;
		delay = parse_ushort(argv[3], 10);
		if (delay == USHRT_MAX)
			return 1;
		printf("video::video_test_init(%u, %u)\n", mode, delay);
		return video_test_init(mode, delay);
	}

	else if (strncmp(argv[1], "square", strlen("square")) == 0) {
		if (argc != 6) {
			printf(
					"video: wrong no. of arguments for video_test_square(x,y,size,color)\n");
			return 1;
		}
		x = parse_ushort(argv[2], 10);
		if (x == USHRT_MAX)
			return 1;
		y = parse_ushort(argv[3], 10);
		if (y == USHRT_MAX)
			return 1;
		size = parse_ushort(argv[4], 10);
		if (size == USHRT_MAX)
			return 1;
		color = parse_ulong(argv[5], 10);
		if (x == ULONG_MAX)
			return 1;
		printf("video::video_test_square(%u, %u, %u, %lu)\n", x, y, size,
				color);
		return video_test_square(x, y, size, color);

	} else if (strncmp(argv[1], "line", strlen("line")) == 0) {
		if (argc != 7) {
			printf(
					"video: wrong no of arguments for video_test_line(xi, yi, xf, yf, color)\n");
			return 1;
		}

		xi = parse_ushort(argv[2], 10);
		if (xi == USHRT_MAX)
			return 1;
		yi = parse_ushort(argv[3], 10);
		if (yi == USHRT_MAX)
			return 1;
		xf = parse_ushort(argv[4], 10);
		if (xf == USHRT_MAX)
			return 1;
		yf = parse_ushort(argv[5], 10);
		if (yf == USHRT_MAX)
			return 1;
		color = parse_ulong(argv[6], 10);
		if (color == ULONG_MAX)
			return 1;
		printf("video::video_test_line(%u, %u, %u, %u, %lu)\n", xi, yi, xf, yf,
				color);
		return video_test_line(xi, yi, xf, yf, color);

	} else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
		if (argc != 5) {
			printf("video: wrong no. of arguments for test_xpm(xpm, xi, yi)\n");
			return 1;
		}
		xi = parse_ushort(argv[3], 10);
		if (xi == USHRT_MAX)
			return 1;
		yi = parse_ushort(argv[4], 10);
		if (yi == USHRT_MAX)
			return 1;
		xpm = getPixMap(argv[2]);
		if (xpm == NULL) {
			printf("No such pixmap in memory\n");
			return 1;
		}
		printf("video::test_xpm(%u, %u)\n", xi, yi);
		return test_xpm(xpm, xi, yi);
	} else if (strncmp(argv[1], "move", strlen("move")) == 0) {

		if (argc != 9) {
			printf(
					"video: wrong no. of arguments for test_line(xpm, xi, yi, xf, yf, speed, fps)\n");
			return 1;
		}

		xpm = getPixMap(argv[2]);
		if (xpm == NULL) {
			printf("No such pixmap in memory\n");
			return 1;
		}

		xi = parse_ushort(argv[3], 10);
		if (xi == USHRT_MAX)
			return 1;
		yi = parse_ushort(argv[4], 10);
		if (yi == USHRT_MAX)
			return 1;
		xf = parse_ushort(argv[5], 10);
		if (xf == USHRT_MAX)
			return 1;
		yf = parse_ushort(argv[6], 10);
		if (yf == USHRT_MAX)
			return 1;
		speed = parse_short(argv[7], 10);
		if (speed == SHRT_MAX)
			return 1;
		fps = parse_ushort(argv[8], 10);
		if (fps == USHRT_MAX)
			return 1;

		printf("video::test_move(%u, %u, %u, %u, %d, %u)\n", xi, yi, xf, yf,
				speed, fps);
		return test_move(xpm, xi, yi, xf, yf, speed, fps);

	} else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {

		if (argc != 2) {
			printf("video: wrong no. of arguments for test_controller()\n");
			return 1;
		}

		printf("video::test_controller()\n");
		return test_controller();

	}

	else {
		printf("video: %s - no valid function!\n", argv[1]);
		return 1;

	}
}

static unsigned short parse_ushort(char *str, int base) {
	char *endptr;
	unsigned short val;

	/* Convert string to unsigned short */
	val = (unsigned short) strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == USHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return USHRT_MAX;
	}

	if (endptr == str) {
		printf("mouse: parse_ulong: no digits were found in %s\n", str);
		return USHRT_MAX;
	}

	/* Successful conversion */
	return val;
}

static short parse_short(char *str, int base) {
	char *endptr;
	short val;

	/* Convert string to short */
	val = (short) strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == SHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return SHRT_MAX;
	}

	if (endptr == str) {
		printf("mouse: parse_ulong: no digits were found in %s\n", str);
		return SHRT_MAX;
	}

	/* Successful conversion */
	return val;
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	/* Convert string to unsigned long */
	val = strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("mouse: parse_ulong: no digits were found in %s\n", str);
		return ULONG_MAX;
	}

	/* Successful conversion */
	return val;
}

#endif
