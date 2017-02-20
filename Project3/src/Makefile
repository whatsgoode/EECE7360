ABS_DIR = ../../Abstraction
CFLAGS=-g -O0 -Wall -std=c99 -I $(ABS_DIR)
P3_OBJS=main.o $(ABS_DIR)/Subset_Sum.o

all: build

build: abstract proj

abstract: 
	+$(MAKE) -C $(ABS_DIR)

proj: $(P3_OBJS)
	$(CC) $(CFLAGS) -o p3 $(P3_OBJS)

clean:
	rm -f *.so *.o p3

%.o: %.c %.h 
	$(CC) $(CFLAGS) -c -o $@ $<
