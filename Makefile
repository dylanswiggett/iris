# Makefile for Unix/Linux based systems only

CFLAGS := -Wall -lm -I/usr/local/include/SDL -lSDL -lGL -lGLU
FILES := main.cpp
OBJECTS := main.o
MAIN := main
CC := g++
OUTPUT := iris
LEAKFLAGS := --leak-check=full 
LEAKFILE := log/leaklog

help:
	@cat man/makehelp

objects:
	@echo "compiling source..."
	@$(CC) -g $(CFLAGS) -c $(FILES)

exe:
	@echo "linking objects..."
	@$(CC) -g $(CFLAGS) $(OBJECTS) -o $(OUTPUT)

clean:
	@echo "removing objects..."
	@rm $(OBJECTS)

all:
	@make -s objects
	@make -s exe
	@make -s clean
	@echo "compiled $(OUTPUT)!"

run:
	@make all
	@./$(OUTPUT)

leak:
	@make -s all
	@echo "checking $(OUTPUT) for leaks..."
	@valgrind $(LEAKFLAGS) ./$(OUTPUT)

leaklog:
	@make -s all
	@echo "checking $(OUTPUT) for leaks and recording to $(LEAKFILE)"
	@mkdir `echo $(LEAKFILE) | grep -Eo '^[^/]*'`
	@valgrind $(LEAKFLAGS) --log-file=$(LEAKFILE) ./$(OUTPUT)
