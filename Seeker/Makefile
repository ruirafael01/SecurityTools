EXECUTABLE_NAME = seek
TARGET = bin/$(EXECUTABLE_NAME)

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: all

valgrind: all
		  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/$(EXECUTABLE_NAME) -w test_wordlist.txt -u "http://127.0.0.1/"

clean:
	rm -f obj/*.o
	rm -f bin/*

$(TARGET): $(OBJ)
	gcc -o $@ $? -lcurl

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude -I/usr/local/include
all: clean $(TARGET)