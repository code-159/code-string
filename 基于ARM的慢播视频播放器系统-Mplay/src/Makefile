
VPATH = ../include
CC=arm-linux-gcc

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)

CFLAGS = -L ../lib -l jpeg -l font -lm -pthread

$(PROGRAM_NAME):$(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)
	mv $(PROGRAM_NAME) ../

$(OBJ):%.o:%.c
	$(CC) $^ -o $@ -c -I ../include


clean:
	$(RM) *.o $(PROGRAM_NAME)


