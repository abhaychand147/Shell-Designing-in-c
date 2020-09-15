# Define required macros here
SHELL = /bin/sh

CFLAG = -Wall -g
CC = gcc
INCLUDE =
LIBS = -lm

all:
	${CC} ${CFLAGS} myshell.cpp -lreadline -o myshell.o

clean:
	rm -f *.o *.o

