#include "mpi.h"
#include "iostream"

using namespace std;

using namespace std;

int main(int argc, char* argv []) {
	/*Ring program*/
	MPI_Init(&argc, &argv);

	int rank, size;

	int val;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	MPI_Status status; 
	if (rank != 0) {
		MPI_Recv(&val, 1, MPI_INT, rank - 1, rank -1 , MPI_COMM_WORLD, &status);
		cout << "Process " << rank << " received " << val << " from process #"<<status.MPI_SOURCE<<"\n";
		val += 1;
	}
	else {
		val = -1; 
	}
  
	MPI_Send(&val, 1, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
	if (rank == 0){
		MPI_Recv(&val, 1, MPI_INT, size - 1, size - 1, MPI_COMM_WORLD, &status);
		cout << " Process " << rank << " received value " << val << " from process " << status.MPI_SOURCE << "\n";
	}

	MPI_Finalize();
}
