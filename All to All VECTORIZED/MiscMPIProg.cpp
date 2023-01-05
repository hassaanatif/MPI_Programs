/*MPI All to All Vectorized*/
#include "mpi.h"
#include "iostream"

using namespace std;

int main(int argc, char** argv) {
 
    MPI_Comm comm;
    MPI_Status status;
    int size, rank;

    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);
     
    int N = (size + 1) * 4;
    int* sendBuff = new int[N];
    int* recvBuff = new int[N];
    int* disp = new int[size];
    int* counts = new int[size];
    int* recvCounts = new int[size];
    int* recvDisp = new int[size];
    
    for (int i = 0; i < N; ++i) {
        sendBuff[i] = i;
    }
    for (int i = 0; i < size; ++i) {
        if (i == 0) {
            counts[0] = 3;
            recvCounts[i] = counts[i];
            disp[0] = 0;
            recvDisp[0] = disp[0];
        }
        else {
            counts[i] = 3;
            recvCounts[i] = counts[i];
            disp[i] = counts[i - 1] + disp[i - 1] + 1;
            recvDisp[i] = disp[i];
        }
    }

    MPI_Alltoallv(sendBuff, counts, disp, MPI_INT, recvBuff, recvCounts, recvDisp, MPI_INT, MPI_COMM_WORLD);
    
    cout << "Elements for process#" << rank << " are : \n";
    for (int i = 0; i < counts[rank]; ++i) {
        cout << recvBuff[i];
    }
        delete[] sendBuff;
    delete[] disp;
    delete[] recvBuff;
    delete[] recvCounts;
    delete[] recvDisp;

        MPI_Finalize();
    return 0;
}