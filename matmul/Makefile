all: matmul

matmul: matmul.c
	gcc $^ -o $@ -lpthread -lm -std=c99

clean:
	rm -f *~
	rm -f *.o
	rm -f matmul
