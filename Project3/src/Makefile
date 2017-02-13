CFLAGS=-g -O0 -Wall -std=c99
P2_OBJS=main.o Subset_Sum.o

all: build

build: proj

proj: $(P2_OBJS)
	$(CC) $(CFLAGS) -o p2 $(P2_OBJS)

clean:
	rm -f *.so *.o p1

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
