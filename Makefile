
CC      = gcc

SRCS    = test.c BCD.c

HEADERS = BCD.h

CFLAGS  = -I. 

LIBS    = -lm 



test : $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS)  -o $@  $(SRCS) $(LIBS)

clean:
	$(RM) *.s *.o *~ *.lst *.i test


.PHONY: clean

