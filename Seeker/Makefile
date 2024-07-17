EXECUTABLE_NAME = seek
TARGET = bin/$(EXECUTABLE_NAME)

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: all

valgrind: all
		  valgrind --leak-check=full ./bin/$(EXECUTABLE_NAME)

clean:
	rm -f obj/*.o
	rm -f bin/*

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude

all: clean $(TARGET)