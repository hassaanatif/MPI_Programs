/*MPI gather and all gather */
#include "mpi.h"
#include "iostream"

using namespace std;

 
int main(int argc, char* argv[]) {
    const int N = 100;
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the rank and size of the communicator
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // The root process initializes and scatters the array
    double data[N];
    double* portion;
    portion = new double[N / size];


    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            data[i] = (double)i;
        }
        MPI_Scatter(data, N / size, MPI_DOUBLE, MPI_IN_PLACE, N / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }
    // The other processes receive their portion of the array
    else {
        MPI_Scatter(data, N / size, MPI_DOUBLE, portion, N / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        // Print the received portion of the array
        //printf("Process %d received portion: ", rank);
        //for (int i = 0; i < N / size; i++) {
        //    printf("%f ", portion[i]);
        //}
        //printf("\n");

    }

    double* recvdElements = new double[N];

    MPI_Gather(portion, N / size, MPI_DOUBLE, recvdElements, N / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    //MPI_Allgather(portion, N / size, MPI_DOUBLE, recvdElements, N/size,MPI_DOUBLE, MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Received elements of proc "<<rank<<" are : \n";
        for (int i = (rank + 1 * N/size); i < N; ++i) {
            cout<<recvdElements[i] << "\n";
        }
    }

    delete[] recvdElements;
    delete[] portion;

     

    // Finalize MPI
    MPI_Finalize();

    return 0;
}