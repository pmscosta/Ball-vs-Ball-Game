# Makefile for lab2

COMPILER_TYPE= gnu

CC= gcc

PROG= lab3
SRCS= test3.c lab3.c kbc_functions.c kbc_IH_asm.S timer_functions.c

CFLAGS= -Wall -D LAB3


DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys


LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
