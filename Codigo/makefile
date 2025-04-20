
all: v1 v2f v2b v2b2 v2d v3 v4

v1: v1.c
	$(CC) $(CFLAGS) -Wall -o v1 v1.c -lm
	
v2f: v2_fusion.c
	$(CC) $(CFLAGS) -Wall -o v2f v2_fusion.c -lm

v2b: v2_bloques.c
	$(CC) $(CFLAGS) -Wall -o v2b v2_bloques.c -lm

v2b2: v2_bloques2.c
	$(CC) $(CFLAGS) -Wall -o v2b2 v2_bloques2.c -lm

v2d: v2Desenrrollamiento.c
	$(CC) $(CFLAGS) -Wall -o v2d v2Desenrrollamiento.c -lm
	
v3: v3.c
	$(CC) $(CFLAGS) -mavx2 -mfma -Wall -o v3 v3.c -lm
	
v4: v4.c
	$(CC) $(CFLAGS) -fopenmp -Wall -o v4 v4.c -lm

run: v1 v2f v2b v2b2 v2d v3 v4
	./v1 $(ARGS)
	./v2f $(ARGS)
	./v2b $(ARGS)
	./v2b2 $(ARGS)
	./v2d $(ARGS)
	./v3 $(ARGS)
	./v4 $(ARGS)

clean:
	rm -f v1 v2f v2b v2b2 v2d v3 v4