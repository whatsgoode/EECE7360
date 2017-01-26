CFLAGS=-g -O0 -Wall -std=c99
P1_OBJS=main.o Subset_Sum.o

all: build

build: proj

proj: $(P1_OBJS)
	$(CC) $(CFLAGS) -o p1 $(P1_OBJS)

clean:
	rm -f *.so *.o p1

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<
