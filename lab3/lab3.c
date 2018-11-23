#ifndef __LAB3_C
#define __LAB3_C

#include "test3.h"
#include <limits.h>
#include  <stdlib.h>
#include <string.h>
#include <errno.h>
#include <minix/syslib.h>

static int proc_args(int argc, char **argv);
static unsigned short parse_ushort(char *str, int base);
static void print_usage(char **argv);
void sef_startup();

int main(int argc, char **argv)
{
	/* DO NOT FORGET TO initialize service */
	sef_startup();

	sys_enable_iop(SELF);

	if (argc == 1) {					/* Prints usage of the program if no arguments are passed */
		print_usage(argv);
		return 0;
	}
	else return proc_args(argc, argv);
}

static void print_usage(char **argv)
{
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"scan zero-> C, non-zero-> ASM>\"\n"
			"\t service run %s -args \"poll <no args>\"\n"
			"\t service run %s -args \"timed_scan <decimal no. - time>\"\n",
			argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char **argv)
{
	unsigned short assembly, n;
	if (strncmp(argv[1], "scan", strlen("scan")) == 0) {
		if (argc != 3) {
			printf("kbd: wrong no. of arguments for kbd_test_scan()\n");
			return 1;
		}
		assembly = parse_ushort(argv[2], 10);						/* Parses string to unsigned long */
		if (assembly == USHRT_MAX)
			return 1;
		printf("kbd::kbd_test_scan( %u )\n", assembly);
		return kbd_test_scan(assembly);
	}
	else if (strncmp(argv[1], "poll", strlen("poll")) == 0) {
		if (argc != 2) {
			printf("kbd: wrong no. of arguments for kbd_test_poll()\n");
			return 1;
		}
		printf("kbd::kbd_test_poll()\n");
		return kbd_test_poll();
	}
	else if (strncmp(argv[1], "timed_scan", strlen("timed_scan")) == 0) {
		if (argc != 3) {
			printf("timer: wrong no of arguments for kbd_test_timed_scan()\n");
			return 1;
		}
		n = parse_ushort(argv[2], 10);						/* Parses string to unsigned long */
		if (n == USHRT_MAX)
			return 1;
		printf("kbd::kbd_test_timed_scan(%u)\n", n);
		return kbd_test_timed_scan(n);
	}
	else {
		printf("kbd: %s - no valid function!\n", argv[1]);
		return 1;

	}
}

static unsigned short parse_ushort(char *str, int base)
{
	char *endptr;
	unsigned short val;

	/* Convert string to unsigned long */
	val = (unsigned short) strtoul(str, &endptr, base);

	/* Check for conversion errors */
	if ((errno == ERANGE && val == USHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtoul");
		return USHRT_MAX;
	}

	if (endptr == str) {
		printf("kbd: parse_ulong: no digits were found in %s\n", str);
		return USHRT_MAX;
	}

	/* Successful conversion */
	return val;
}

#endif
