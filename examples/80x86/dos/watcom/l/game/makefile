OBJECTS:=bsp.o mine1.o mine2.o missile.o ship.o tunnel.o \
video.o main.o
CFLAGS +=-I../../../../../../include/
CFLAGS +=-I../../../../../../ports/80x86/dos/watcom/l/
#CFLAGS +=-I../../../../../../WATCOM/h/
#CFLAGS +=-D_M_IX86
CFLAGS +=-DLMC_TEST_2020_1
CFLAGS +=-I../../../../../../xv6/
CC=gcc
#all:bsp.o
all:$(OBJECTS)
#bsp.o:bsp.c
#	$(CC) $(CFLAGS) -c $<
clean:
	rm -v *.o
