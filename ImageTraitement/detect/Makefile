# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -std=c99 `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS =
LDLIBS = -lm -lrt `pkg-config --libs sdl2 SDL2_image`

SRC = detect.c
OBJ = ${SRC:.c=.o}

detect: ${OBJ}

clean:
	${RM} ${OBJ}
	${RM} detect
# End
