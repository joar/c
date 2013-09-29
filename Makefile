CC=c99
CFLAGS=-I.
DEPS=hellomake.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ls: ls.o
	$(CC) -Wall ls.c -o ls $(CFLAGS)

.PHONY: clean

clean:
	rm -f ./*.o
