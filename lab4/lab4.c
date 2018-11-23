#ifndef __LAB4_C
#define __LAB4_C

#include "test4.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>

static int proc_args(int argc, char **argv);
static unsigned short parse_ushort(char *str, int base);
static unsigned long parse_ulong(char *str, int base);
static short parse_short(char *str, int base);
static void print_usage(char **argv);
void sef_startup();

int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup();

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}



static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_packet <decimal no -packets\"\n"
			"\t service run %s -args \"test_async <decimal no - seconds>\"\n"
			"\t service run %s -args \"test_remote <decimal no. - period> <decimal no. -counter>\"\n"
			"\t service run %s -args \"test_gesture <decimal no. - length>\"\n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned short packets, idle_time, cnt;
	short length;
	unsigned long period;
	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_packet(packets)\n");
			return 1;
		}
		packets = parse_ushort(argv[2], 10);
		if (packets == USHRT_MAX)
			return 1;
		printf("mouse::mouse_test_packet(%u)\n", packets);
		return mouse_test_packet(packets);
	}

	else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {
		if (argc != 3) {
			printf("mouse: wrong no. of arguments for mouse_test_async(idle_time)\n");
			return 1;
		}
		idle_time = parse_ushort(argv[2], 10); /* Parses string to unsigned long */

		if (idle_time == USHRT_MAX)
			return 1;
		printf("mouse_test_async(%u)\n", idle_time);
		return mouse_test_async(idle_time);
	}
	else if (strncmp(argv[1], "test_remote", strlen("test_remote")) == 0) {
		if (argc != 4) {
			printf("mouse: wrong no of arguments for mouse_test_remote(period, cnt)\n");
			return 1;
		}

		period = parse_ulong(argv[2], 10);
		cnt = parse_ushort(argv[3], 10);
		if (period == ULONG_MAX)
			return 1;
		if (cnt == USHRT_MAX)
			return 1;
		printf("mouse::mouse_test_remote(%lu,%u)\n", period, cnt);
		return mouse_test_remote(period, cnt);

	}
	else if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0){
		if (argc != 3){
			printf("mouse: wrong no. of arguments for mouse_test_gesture(length)\n");
		return 1;
		}
		length  = parse_short(argv[2],10);

		if (length == SHRT_MAX)
			return 1;
		printf("mouse_test_gesture(%u)\n", length);
		return mouse_test_gesture(length);
	}

	else {
		printf("mouse: %s - no valid function!\n", argv[1]);
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

static unsigned long parse_ulong(char *str, int base)
{
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
