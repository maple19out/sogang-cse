CC=gcc
CFLAGS=-g -Wall
OBJS=main.o list.o hash.o bitmap.o hex_dump.o debug.o
TARGET=testlib

$(TARGET): $(OBJS)
						 $(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o
	rm -f $(TARGET)
