/*All reduce */
#include "mpi.h"
#include "iostream"

using namespace std;

using namespace std;

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    // Get the rank and size of the communicator
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data[10];
    int reduced[10];
    for (int i = 1; i <= 10; ++i) {
        data[i - 1] = i;
    }

    MPI_Allreduce(data, reduced, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < 10; ++i) {
            cout << reduced[i] << "\n";
        }
    }
    // Finalize MPI
    MPI_Finalize();

    return 0;
}
