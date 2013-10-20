# 
# Makefile for dir
# 

MAKE = make
CC = gcc
RM = rm -f

CSOURCE = dir.c

INCLUDES = -I/usr/include


all: fe even odd ftest
	@echo done

fe.o: fe-test.c 
	@echo "C-compiling 'fe-test.c'"
	
evens.o: evens.c 
	@echo "C-compiling 'evens.c'"
	
ftest.o: f-test.c 
	@echo "C-compiling 'f-test.c'"
	
odds.o: odds.c 
	@echo "C-compiling 'odds.c'"


ftest: $(APPL_OBJECTS)
	@echo Building executable file
	@$(CC) -o all $(APPL_OBJECTS)
	@echo "Executable 'fork' made"

clean::
	@$(RM) *.o
	@$(RM) all


