PROGRAM = tower-of-hanoi

CC     = gcc
CFLAGS = -Wall

SRC = main.c
EXE = $(PROGRAM)
ZIP = $(PROGRAM).zip

all: clean
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

run:
	./$(EXE)

clean:
	rm -f *~
	rm -f $(EXE)
	rm -f $(ZIP)

x: clean all run

zip: clean
	zip -r $(ZIP) *

memcheck: all
	valgrind --tool=memcheck --leak-check=full ./$(EXE)

