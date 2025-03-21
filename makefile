
all: v1 v2

v1: v1.c
	$(CC) $(CFLAGS) -Wall -o v1 v1.c -lm
	
v2: v2.c
	$(CC) $(CFLAGS) -Wall -o v2 v2.c -lm
	
run: v1 v2
	./v1 $(ARGS)
	./v2 $(ARGS)

clean:
	rm -f v1 v2