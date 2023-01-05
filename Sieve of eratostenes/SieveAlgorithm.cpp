/*Parallel Sieve Prime Finder 1/1/23*/
#include "iostream"
#include "stdlib.h"
#include "mpi.h"
#define BLOCK_LOW(id, p, n)  ((id) * (n/p))
#define BLOCK_HIGH(id, p, n) BLOCK_LOW(id, p, n) - 1
#define BLOCK_SIZE (id, p, n)  (BLOCK_LOW(id + 1, p, n) - BLOCK_LOW(id, p, n))
#define BLOCK_OWNER(index, p, n) ((p) * ((index) + 1) - 1)/(n))

int main(int argc , char **argv) {
	int id, p = 0; 

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);


	int n = 9; //sieve up to this number
	int low_value = BLOCK_LOW(id,p, n - 1);
	int high_value = BLOCK_HIGH(id, p, n - 1);
	int size_of_block = BLOCK_LOW(id + 1, p, n) - BLOCK_LOW(id, p, n);
	int first; 

	int proc0_size = (n - 1) / p; 

	if (proc0_size + 2 < (int)sqrt((double)n)) {
		if (!id) printf("Too many processes!\n");
		MPI_Finalize();
		exit(1);
	}

	char* marked = new char[size_of_block];

	int prime = 2; 
	int index;
	
	for (int i = 0; i < size_of_block; ++i) {
		marked[i] = 0;
	}
	if (!id) index = 0; 

	do {
		if (prime * prime > low_value) {
			first = prime * prime - low_value;
		}
		else {
			if (!(low_value % prime)) first = 0;
			else first = prime - (low_value % prime);
		}

		for (int i = first; i < size_of_block; i += prime) marked[i] = 1; 
		
		if (!id) {
			while (marked[++index]);
			prime = index + 2; 
		}
		MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
	} while (prime * prime <= sqrt(n));

	int count = 0; 
	int global_count = 0;
	for (int i = 0; i < size_of_block; ++i) {
		if (!(marked[i])) count++;
	
		if (!(marked[i]))
		std::cout << "proc#" << id << " index #" << i << std::endl;
	}
	MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (!id) {
		std::cout << "Total number of primes are : " << global_count;
	}

	MPI_Finalize();
}