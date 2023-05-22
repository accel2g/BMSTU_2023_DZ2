CC = g++
CFLAGS = -c -Wall

all: DZ2

DZ2: main.o hw2.o
	$(CC) main.o hw2.o -o DZ2

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

hw2.o: hw2.cpp
	$(CC) $(CFLAGS) hw2.cpp

clean:
	rm -rf *.o DZ2