CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

project: project.o ppm_io.o imageManip.o
	$(CC) project.o ppm_io.o imageManip.o -o project -lm

project.o: project.c imageManip.h ppm_io.h
	$(CC) $(CFLAG) -c project.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAG) -c ppm_io.c

imageManip.o: imageManip.c imageManip.h
	$(CC) $(CFLAG) -c imageManip.c

clean:
	rm -f *.o project
