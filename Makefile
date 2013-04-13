
CC      = gcc

SRCS    = test.c BCD.c

HEADERS = BCD.h

CFLAGS  = -I. 

LIBS    = -lm 



run: test
	./test

test : $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS)  -o $@  $(SRCS) $(LIBS)

clean:
	$(RM) *.s *.o *~ *.lst *.i test

again:
	make clean
	make

.PHONY: clean run again

floatdec: floatdec.c
	$(CC) $(CFLAGS) -o $@ floatdec.c -lm

