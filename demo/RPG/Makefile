SGLDIR=../../src
EXEC=target
CC=gcc
CFLAGS= -Wall -std=c11 `sdl2-config --cflags` -I$(SGLDIR)
LDFLAGS= `sdl2-config --libs` -lSDL2_image -lSDL2_mixer

TARGET := $(notdir $(CURDIR))
SOURCEDIR := src
SOURCE := $(SGLDIR)/init.c \
$(SGLDIR)/UI/dialog.c \
$(SGLDIR)/fontmap.c \
$(SGLDIR)/image.c \
$(SGLDIR)/tileset.c \
$(SGLDIR)/tilemap.c \
$(SGLDIR)/sprite.c \
$(SGLDIR)/animation.c \
$(SGLDIR)/sound.c \
$(foreach DIR,$(SOURCEDIR),$(wildcard $(DIR)/*.c)) \
$(foreach DIR,$(SOURCEDIR),$(wildcard $(DIR)/levels/*.c))

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
