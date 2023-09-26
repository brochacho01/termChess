CC = g++
CFLAGS = -g -Wall -Wextra

OBJS = networkUtilities.o driver.o board.o gameHelpers.o ./pieces/piece.o ./pieces/pawn.o ./pieces/rook.o ./pieces/knight.o ./pieces/bishop.o ./pieces/queen.o ./pieces/king.o ./pieces/checkLogic.o

all: build

build: $(OBJS)
	$(CC) $(CFLAGS) -o termChess $(OBJS)

clean:
	rm -f termChess $(OBJS)

run: build
	./termChess

$(OBJS) : networkUtilities.h board.h gameHelpers.h constants.h ./pieces/piece.h