ABS_DIR = ../../Abstraction
CFLAGS=-g -O0 -Wall -std=c99 -I $(ABS_DIR)
P5_OBJS=main.o $(ABS_DIR)/Subset_Sum.o

all: build

build: abstract proj

abstract: 
	+$(MAKE) -C $(ABS_DIR)

proj: $(P5_OBJS)
	$(CC) $(CFLAGS) -o p5 $(P5_OBJS)

clean:
	rm -f *.so *.o p5

%.o: %.c %.h 
	$(CC) $(CFLAGS) -c -o $@ $<
