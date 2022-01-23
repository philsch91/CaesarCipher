CC=gcc
#CFLAGS=-I.

#%.o: %.c $(DEPS)
	#$(CC) -c -o $@ $< $(CFLAGS)

all: dir clean binary

dir:
	mkdir -pv bin

binary: caesarcipher.o
	$(CC) -o bin/caesarcipher bin/caesarcipher.o

caesarcipher.o: CaesarCipher/caesarcipher.h CaesarCipher/caesarcipher.c
	$(CC) -c CaesarCipher/caesarcipher.c -o bin/caesarcipher.o

clean:
	-rm -f bin/caesarcipher bin/caesarcipher.o
