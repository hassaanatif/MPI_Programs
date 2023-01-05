/*MPI All to All*/
#include "mpi.h"
#include "iostream"

using namespace std;

int main(int argc, char** argv) {
    int rank, size, i;
    int sendbuf[20], recvbuf[20];
    MPI_Comm comm;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    /* Initialize the send buffer with the rank of the process */
    for (i = 0; i < 20; i++)
        sendbuf[i] = i;

    /* Set up an array of 10 element counts and 10 displacements */
    int *sendcounts = new int[size], *sdispls = new int [size], *recvcounts = new int[size],
                       *rdispls = new int[size];
    for (i = 0; i < size; i++) {
        sendcounts[i] = i;
        sdispls[i] = i;
        recvcounts[i] = 1;
        rdispls[i] = i;
    }

     MPI_Datatype* sendTypes = new int[size];
     MPI_Datatype* recvTypes = new int[size];

    for (int i = 0; i < size; ++i) {
        sendTypes[i] = MPI_INT;
        recvTypes[i] = MPI_INT;
    }
    /* Perform the all-to-all operation */

    //MPI_Alltoallw(sendbuf, sendcounts, sdispls, sendTypes,
    //    recvbuf, recvcounts, rdispls, recvTypes, comm);

    MPI_Alltoall(sendbuf, 20/size, MPI_INT, recvbuf, 20/size, MPI_INT, MPI_COMM_WORLD);

    /* Print the received data */
    printf("Process %d received: ", rank);
    for (i = 0; i < 20/size; i++)
        printf("%d ", recvbuf[i]);
    printf("\n");
     
    delete[] sendTypes;
    delete[] recvTypes;
    delete[] sendcounts;
    delete[] recvcounts;
    delete[] sdispls;
    delete[] rdispls;

    MPI_Finalize();
    return 0;
}