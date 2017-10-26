# Project name
NAME=puzzle15_astar

# Finally something easy to read!
CC=g++
CFLAGS= -std=c++0x -O3 -g -Wall 

all: clean main

main: 
	@echo Compiling...
	@$(CC) main.cpp -o $(NAME) $^ $(CFLAGS)


# Run directives
run:
	./$(NAME) $(filter-out $@,$(MAKECMDGOALS))
%:
	@:

# Utility directives
clean:
	rm -f $(NAME)
	clear

tar: clean
	@echo Compressing files...
	@tar -zcvf $(NAME).tar.gz *
	@echo Done.

zip: clean
	@echo Compressing files...
	@zip -r $(NAME).zip *
	@echo Done.
