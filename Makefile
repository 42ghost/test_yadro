CC = g++
CFLAGS =

all: task clean

task: task.o ComputerClub.o
	$(CC) $(CFLAGS) -o task.exe task.o ComputerClub.o Computer.o

task.o: task.cpp
	$(CC) $(CFLAGS) -c task.cpp -o task.o

ComputerClub.o: ComputerClub.cpp ComputerClub.hpp Computer.o
	$(CC) $(CFLAGS) -c ComputerClub.cpp

Computer.o: Computer.cpp Computer.hpp
	$(CC) $(CFLAGS) -c Computer.cpp

clean:
	rm -f *.o
