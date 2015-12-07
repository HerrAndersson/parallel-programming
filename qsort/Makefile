all: qsort

qsort: qsort.c
	gcc $^ -o $@ -lpthread -std=c99

clean:
	rm -f *~
	rm -f *.o
	rm -f qsort
