# Makefile for ballvsball

COMPILER_TYPE= gnu

CC=gcc

PROG= ballvsball

SRCS= proj.c mouse.c video_gr.c vbe.c rtc.c highscore.c timer.c keyboard.c bitmap.c player.c rectangle.c time.c ball.c vector.c

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}

LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>

.include <bsd.gcc.mk>