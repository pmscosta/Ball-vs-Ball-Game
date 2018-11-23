# Makefile for lab5

COMPILER_TYPE= gnu

CC=gcc

PROG= lab5

SRCS= lab5.c vbe.c video_gr.c timer_functions.c test5.c video_test.c kbd_functions.c pixmap.c sprite.c

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}

LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>

.include <bsd.gcc.mk>