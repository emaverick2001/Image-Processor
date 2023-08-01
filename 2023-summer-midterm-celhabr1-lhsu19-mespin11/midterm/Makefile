# Maverick Espinosa mespin11
# Colton El-Habr celhabr1
# Leo Hsu lhsu19

# Midterm Project - Solution Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

## TODO: Write your own Makefile commands to link and compile your project as well as the cleanup


## Below are commands to link and compile the project program
# Links together files needed to create the project executable
project : project.o ppm_io.o img_processing.o
	$(CC) -o $@ project.o ppm_io.o img_processing.o

# Compile the ppm i/o source code
project.o: project.c ppm_io.h img_processing.h
	$(CC) $(CFLAGS) -c project.c

# Compile the ppm i/o source code
ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

# Compile the ppm i/o source code
img_processing.o: img_processing.c ppm_io.h img_processing.h
	$(CC) $(CFLAGS) -c img_processing.c

# Removes all object files and the executable named project, so we can start fresh
clean:
	rm -f *.o project
