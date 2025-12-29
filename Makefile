CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iheaders

SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS   = $(shell sdl2-config --libs)

SRC = \
    main.c \
    physics.c \
    render.c \
    input_drag.c \
    sdl_app.c

OBJ = $(SRC:.c=.o)
OBJ := $(addprefix build/, $(OBJ))

TARGET = build/game

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(SDL_LIBS) -lm

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

clean:
	rm -rf build
