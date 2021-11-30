CC = gcc
CFLAGS = -g
LIBFLAGS = 
CLIENT = client 
SERVER = server 

all: $(CLIENT) $(SERVER)

client: client.o
	@$(CC) -o $@ $^ $(LIBFLAGS)

server: server.o
	@$(CC) -o $@ $^ $(LIBFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $^ $(CFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o

