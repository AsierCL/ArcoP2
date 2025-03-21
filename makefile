
all: v1

v1: v1.c
	$(CC) $(CFLAGS) -lm -Wall -o v1 v1.c


run: v1
	./v1 $(ARGS)

clean:
	rm -f v1