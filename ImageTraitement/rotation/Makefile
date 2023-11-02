# Makefile


CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -lrt


SRC = rotate.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

rotate: rotate.o

main: ${OBJ}


.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# End
