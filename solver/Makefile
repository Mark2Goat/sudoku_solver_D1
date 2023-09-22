# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2` -lm

all: solver

SRC = solver.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
