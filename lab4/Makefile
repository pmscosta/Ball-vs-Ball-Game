# Makefile for lab4

COMPILER_TYPE= gnu

CC= gcc

PROG= lab4
SRCS= test4.c lab4.c mouse_functions.c timer_functions.c kbd_functions.c

CFLAGS= -Wall -D LAB4


DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys


LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
