CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

.PHONY: all clean format

all: encode decode

encode: encode.o huffman.o code.o pq.o node.o stack.o io.o
	$(CC) -o encode encode.o huffman.o code.o pq.o node.o stack.o io.o $(LFLAGS)

decode: decode.o huffman.o code.o pq.o node.o stack.o io.o
	$(CC) -o decode decode.o huffman.o code.o pq.o node.o stack.o io.o $(LFLAGS)

encode.o: encode.c 
	$(CC) $(CFLAGS) -c encode.c 

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c

code.o: code.c
	$(CC) $(CFLAGS) -c code.c

pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

clean:
	rm encode decode *.o

format:
	clang-format -i -style=file *.[ch]

