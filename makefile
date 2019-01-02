CC = gcc
CFLAGS = -Wall
OBJ = main.o list.o

# Main target

program: $(OBJ)
	$(CC) -o $@ $(OBJ)

# Building targets

main.o: main.c
	$(CC) -c $? $(CFLAGS)

list.o: list.c
	$(CC) -c $? $(CFLAGS)

# Utility targets

memory-check: program
	valgrind ./program

line-count: list.c
	clang-format $? > formatted_list.c
	cloc formatted_list.c

clean:
	rm *.o program