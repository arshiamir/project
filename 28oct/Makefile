CC=gcc
CFLAGS=-W -Wall -ansi -std=c99 -g
LIBS = -lSDL2_ttf -lSDL2_image -lSDL2 -L./SDL2_ttf/.libs -L./SDL2_image/.libs 
LDFLAGS=`sdl2-config --cflags --libs` #
INCLUDES = -I./SDL2_ttf -I./SDL2_image
EXEC=main
SRC= main.c fonctions_fichiers.c
OBJ= $(SRC:.c=.o)
all: $(EXEC)
main: $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) $(INCLUDES) -o $@  $(LIBS) 
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)