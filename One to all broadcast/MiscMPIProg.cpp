/*One to all broadcast*/
#include "mpi.h"
#include "iostream"

using namespace std;

using namespace std;

int main(int argc, char* argv []) {
    MPI_Init(&argc, &argv);

    // Get the rank and size of the communicator
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data[10];
    if (rank == 0) {
        for (int i = 0; i < 10; ++i) {
            data[i] = i;
        }
    }

    MPI_Bcast(data, 10, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 10; ++i) {
        cout << "proc#" << rank << " element " << i << " : " << data[i] << "\t";
    }
    cout << "\n";
    // Finalize MPI
    MPI_Finalize();

    return 0;
}
