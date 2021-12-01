CC = gcc
CFLAGS = -g
LIBFLAGS = 
PARSING = parsing 
JOBS = jobs 
BUILTIN = builtin

shell: shell.o parsing.o jobs.o builtin.o
	@$(CC) -o $@ $^ $(LIBFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $^ $(CFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o