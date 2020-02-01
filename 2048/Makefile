CC = gcc
CPPC = g++
FC = gfortran

c2048 : game.cpp
	${CPPC} game.cpp -o c2048

f2048 : 2048.f95 _console.c
	$(CC) -c _console.c -o console.c.o
	$(FC) -c 2048.f95 -o 2048.f95.o
	$(FC) 2048.f95.o console.c.o -o f2048
