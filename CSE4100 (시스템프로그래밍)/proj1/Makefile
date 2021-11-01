CC=gcc
CFLAGS=-g -Wall
OBJS=20161559.o
TARGET=20161559.out

$(TARGET) : $(OBJS)
			  $(CC) -o $@ $(OBJS)

20161559.o: 20161559.h 20161559.c

clean:
	rm -f *.o
	rm -f $(TARGET)
