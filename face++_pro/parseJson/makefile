
##makefile for face++

CC=gcc
TARGET=cJson
INCLUDE=.
FLAG=-Wall
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
LIB+=libcurl.so libcurl.a

all:
	$(CC) $(FLAG) $(SRC) -o $(TARGET) -lm -L. $(LIB)

 


clean:
	rm $(OBJ) $(TARGET) -fr
