# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm

all: ImageTraitement

SRC = ImageTraitement.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

ImageTraitement: ImageTraitement.o

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
