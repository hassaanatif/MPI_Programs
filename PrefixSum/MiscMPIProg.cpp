/*MPI Prefix sum*/
#include "mpi.h"
#include "iostream"

using namespace std;

 
#define N 10

int main(int argc, char* argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank and size of the communicator
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize an array of integers on each process
    int data[N] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // Compute the prefix sum of the array using MPI_Scan
    int prefix_sum[N];
    MPI_Scan(data, prefix_sum, N, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Print the prefix sum on each process
    printf("Process %d prefix sum: ", rank);
    for (int i = 0; i < N; i++) {
        printf("%d ", prefix_sum[i]);
    }
    printf("\n");

    // Finalize MPI
    MPI_Finalize();

    return 0;
}