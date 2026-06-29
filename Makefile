CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

AR = ar
ARFLAGS = rcs

LIB = libipc.a

SRC = src/ipc.c \
      src/semaphore.c \
      src/transport.c \
      src/setup.c

OBJ = $(SRC:.c=.o)

all: $(LIB) client server init clean

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

client: client.c $(LIB)
	$(CC) $(CFLAGS) client.c -L. -lipc -pthread -o client

server: server.c $(LIB)
	$(CC) $(CFLAGS) server.c -L. -lipc -pthread -o server

init: init.c $(LIB)
	$(CC) $(CFLAGS) init.c -L. -lipc -pthread -o init

clean: cleanup.c $(LIB)
	$(CC) $(CFLAGS) cleanup.c -L. -lipc -pthread -o cleanup

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./init

clean-build:
	rm -f src/*.o *.o *.a client server init cleanup
