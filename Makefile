CC = gcc
CFLAGS = -Wall -Wextra
INCLUDES = -I/usr/include/SDL2/
LIBS = -lSDL2
SRC = snake.c
OBJ = $(SRC:.c=.o)
MAIN = snake

$(MAIN): $(OBJ)
	$(CC) $(OBJ) $(INCLUDES) -o $(MAIN) $(LIBS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $(OBJ)
# $< first requirement

clean:
	rm $(OBJ)
