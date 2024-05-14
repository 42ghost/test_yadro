CC = g++

all: task clean

task: task.o ComputerClub.o
	#$(CC) -g -o task task.o ComputerClub.o Computer.o
	$(CC) -o task.exe task.o ComputerClub.o Computer.o

task.o: task.cpp
	$(CC) -c task.cpp -o task.o

ComputerClub.o: ComputerClub.cpp ComputerClub.hpp Computer.o
	#$(CC) -c -g ComputerClub.cpp
	$(CC) -c ComputerClub.cpp

Computer.o: Computer.cpp Computer.hpp
	#$(CC) -c -g Computer.cpp
	$(CC) -c Computer.cpp

clean:
	rm -f *.o
