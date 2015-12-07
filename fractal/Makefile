all: fractal

fractal: fractal.c
	gcc $^ -o $@ -lpthread -lm -std=c99

clean:
	rm -f *~
	rm -f *.o
	rm -f fractal fracout.tga
