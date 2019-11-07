CFLAGS +=-I../../../../../../include/
CFLAGS +=-I../../../../../../ports/80x86/dos/watcom/l/
#CFLAGS +=-I../../../../../../WATCOM/h/
#CFLAGS +=-D_M_IX86
CFLAGS +=-DLMC_TEST_2020_1
CFLAGS +=-I../../../../../../xv6/
CC=gcc
all:bsp.o
bsp.o:bsp.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -v *.o
