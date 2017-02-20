CFLAGS=-g -O0 -Wall -std=c99
ABS_OBJS=Subset_Sum.o

all: $(ABS_OBJS)

%.o: %.c %.h 
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.so *.o

