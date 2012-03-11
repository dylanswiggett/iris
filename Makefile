# Makefile for Unix/Linux based systems only

CFLAGS := -Wall -lm -I/usr/local/include/SDL
FILES := main.cpp
OBJECTS := main.o
MAIN := main
CC := g++
OUTPUT := iris

objects:
	@$(CC) -g $(CFLAGS) -c $(FILES)

exe:
	@$(CC) -g $(CFLAGS) $(OBJECTS) -o $(OUTPUT)

clean:
	@rm $(OBJECTS)

all:
	@make -s objects
	@make -s exe
	@make -s clean
	@echo "Completed $(OUTPUT)"