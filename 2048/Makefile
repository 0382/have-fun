CC = gcc
CPPC = g++
FC = gfortran
cf_dir = c-fortran

c2048 : ${cf_dir}/game.cpp
	${CPPC} ${cf_dir}/game.cpp -o c2048

f2048 : ${cf_dir}/2048.f95 ${cf_dir}/_console.c
	$(CC) -c ${cf_dir}/_console.c -o console.c.o
	$(FC) -c ${cf_dir}/2048.f95 -o 2048.f95.o
	$(FC) 2048.f95.o console.c.o -o f2048
