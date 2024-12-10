CC = gcc
CFLAGS = -Wall -g
LIBS = -lrabbitmq
INCLUDE = -I/opt/homebrew/Cellar/rabbitmq-c/0.15.0/include
LIBDIR = -L/opt/homebrew/Cellar/rabbitmq-c/0.15.0/lib

SRC = main.c 
OBJ = $(SRC:.c=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LIBDIR) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $<

clean:
	rm -f $(OBJ) $(EXEC)
