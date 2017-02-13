CFLAGS=-g -O0 -Wall -std=c99
P2_OBJS=main.o Subset_Sum.o

all: build

build: proj

proj: $(P3_OBJS)
	$(CC) $(CFLAGS) -o p3 $(P3_OBJS)

clean:
	rm -f *.so *.o p3

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
