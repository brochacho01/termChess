CC = g++
CFLAGS = -g -Wall -Wextra

OBJS = networkUtilities.o driver.o board.o ./pieces/piece.o ./pieces/pawn.o ./pieces/rook.o ./pieces/knight.o ./pieces/bishop.o ./pieces/queen.o ./pieces/king.o

all: build

build: $(OBJS)
	$(CC) $(CFLAGS) -o termChess $(OBJS)

clean:
	rm -f termChess $(OBJS)

run: build
	./termChess

$(OBJS) : networkUtilities.h board.h ./pieces/piece.h