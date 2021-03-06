SGLDIR=../../src
EXEC=target
CC=gcc
CFLAGS= -Wall -std=c17 `sdl2-config --cflags` -I$(SGLDIR)
LDFLAGS= `sdl2-config --libs` -lSDL2_image

TARGET := $(notdir $(CURDIR))
SOURCEDIR := src
SOURCE := $(SGLDIR)/init.c $(SGLDIR)/image.c $(SGLDIR)/fontmap.c $(SGLDIR)/UI/menu.c $(foreach DIR,$(SOURCEDIR),$(wildcard $(DIR)/*.c))

DEBUG=no
ifeq ($(DEBUG),yes)
	CPPFLAGS = -DDEBUG -g
endif

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

all: $(EXEC)

target: $(OBJ)
	$(CC) $(SOURCE) -o $(TARGET) $^ $(LDFLAGS) $(CFLAGS) $(CPPFLAGS)

.PHONY: clean

clean:
	rm $(TARGET)