#include "mpi.h"
#include "stdlib.h"
#include "stdio.h"

void compareSplitFault(int* localElements, int* receivedElements, int n, int keepSmall) {
	int* workingSpace = (int*)malloc(sizeof(int) * n);

	for (int i = 0; i < n; ++i) {
		workingSpace[i] = localElements[i]; 
	}
	int j = 0; 
	int z = 0; 
	if (keepSmall == 1) {
		for (int i = 0; i < n; ++i) {
			if ((localElements[z] <= receivedElements[j] && z != n) || j > 0) {
				workingSpace[i] = localElements[z++];
			}
			else  {
				workingSpace[i] = receivedElements[j++];
			} 
		}
	}
	else {
		j = n - 1; 
		z = n - 1;
		for (int i = n -1; i >= 0; --i) {
			if ((localElements[j] >= receivedElements[z] && j >= 0) || z < 0) {
				workingSpace[i] = localElements[j--];
			}
			else {
				workingSpace[i] = receivedElements[z--];
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		localElements[i] = workingSpace[i];
	}
	free(workingSpace);
}
void compareSplit2(int* localElements, int* receivedElements, int n, int keepSmall) {
	int* temporarySpace = (int*)malloc(sizeof(int) * n);
	int* finalSpace = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; ++i) {
		temporarySpace[i] = localElements[i];
	}
	 
	int i = 0, j = 0, k = 0;
	if (keepSmall == 1) {
		for (; i < n; ++i) {
			if (j == n || ((k < n) && localElements[k] <= receivedElements[j])) {
				finalSpace[i] = localElements[k++];
			}
			else
				finalSpace[i] = receivedElements[j++];
		}
	}
	else {
		for (i = k = j = n - 1; i >= 0; --i) {
			if (j == -1 || (k != -1 && localElements[k] >= receivedElements[j])) {
				finalSpace[i] = localElements[k--];
			}
			else
				finalSpace[i] = receivedElements[j--];
		}
	}
	/**overwrite to orignal array**/
	for (i = 0; i < n; ++i) {
		localElements[i] = finalSpace[i];
	}
	free(finalSpace);
	free(temporarySpace);
}

/*debug*/
void compareSplit(int* localElements, int* receivedElements, int n, int keepSmall) {
	int* workingSpace = (int*)malloc(sizeof(int) * n);

	if (keepSmall == 1) {
		int i = 0, j = 0, k = 0;
		for (; i < n; ++i) {
			if (j == n || (k < n && localElements[k] < receivedElements[j])) {
				workingSpace[i] = localElements[k++];
			}
			else {
				workingSpace[i] = receivedElements[j++];
			}
		}
	}
	else {
		//we want 'n' larger elements
		int i, j, k;
		for (i = j = k = n - 1; i >= 0; --i) {
			if (j != -1 && (k >= 0 && localElements[k] >= receivedElements[j])) {
				workingSpace[i] = localElements[k--];
			}
			else {
				workingSpace[i] = receivedElements[j--];
			}
		}
	}
	for (int i = 0; i < n; ++i) {
		localElements[i] = workingSpace[i];
	}
	free(workingSpace);
}

int mainoddeven(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);

	int rank, p;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	int N = 20; //array size
	int myArr[20] = { 1, 19, 20, 2, 18, 16, 3, 5, 4, 17, 12, 15, 14, 8, 9, 11, 10, 6, 13, 10 };

	int start_array = rank * (N / p);
	int end_array = (rank + 1) * (N / p) - 1;
	//int block_size = (rank + 1) * (N / p) - (rank) * (N / p) - 1;
	int block_size = N / p;

	int* localelements = (int*)malloc(sizeof(int) * block_size);
	int* receivedelements = (int*)malloc(sizeof(int) * block_size);


	MPI_Status status;
	if (rank == 0) {

		/*for (int i = 1; i < p; ++i) {
			MPI_Send((myArr + (i * N / p)), block_size, MPI_INT, i, 0, MPI_COMM_WORLD);
		}*/

	/*	for (int i = 0; i < block_size; ++i) {
			localelements[i] = myArr[i];
		}*/
	}
	else {
		//MPI_Recv(localelements, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		// 
		//MPI_Gather(myArr, N/p, MPI_INT, localelements, N/p, MPI_INT, 0, MPI_COMM_WORLD);

	}
	MPI_Scatter(myArr, block_size, MPI_INT, localelements, block_size, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	int evenrank, oddrank;

	if (!(rank % 2)) {
		evenrank = rank + 1;
		oddrank = rank - 1; 
	}
	else {
		evenrank = rank - 1;
		oddrank = rank + 1;
	}


	if (evenrank == -1 || evenrank == p) {
		evenrank = MPI_PROC_NULL;
	}
	if (oddrank == p || oddrank == -1) {
		oddrank = MPI_PROC_NULL; 
	}

	/*serial sort*/
	int even = 1;
	for (int i = 0; i < block_size; ++i) {
		if (even == 1) {

			for (int j = 0; j < block_size - 1; j += 2) {
			
				if (localelements[j] > localelements[j+1]) {
					int tmp = localelements[j];
					localelements[j] = localelements[j + 1];
					localelements[j + 1] = tmp;
				}
			}
			even = 0;
		}
		else {

			for (int j = 1; j < block_size - 1; j += 2) {

				if (localelements[j] > localelements[j + 1]) {
					int tmp = localelements[j];
					localelements[j] = localelements[j + 1];
					localelements[j + 1] = tmp;
				}
			}
			even = 1;
		}
	}
	/*printf("Hello I am proc %d\t", rank);
	for (int i = 0; i < block_size; ++i) {
		printf("%d\t", localelements[i]);
	}*/

	printf("\n");
	MPI_Barrier(MPI_COMM_WORLD);
	/*parallel algo*/
	for (int i = 0; i < p - 1; ++i) {
		if (!(i % 2)) { //even
			MPI_Sendrecv(localelements, block_size, MPI_INT, evenrank, 1, receivedelements, block_size,
				MPI_INT, evenrank, 1, MPI_COMM_WORLD, &status); //even rank will receive it and even rank will send its share of elements
		}
		else { //odd
			MPI_Sendrecv(localelements, block_size, MPI_INT, oddrank, 1, receivedelements, block_size,
				MPI_INT, oddrank, 1, MPI_COMM_WORLD, &status);
		}
	
		if ((status.MPI_SOURCE != -1 && status.MPI_SOURCE != p ))
		   compareSplit(localelements, receivedelements, block_size, rank < status.MPI_SOURCE? 1 : 0);
	}

	//if (rank != 0) {
	//	MPI_Send(localelements, block_size, MPI_INT, 0, rank, MPI_COMM_WORLD);
	//}
	//else {
	//	for (int i = 1; i < p; ++i) {
	//		MPI_Recv(receivedelements, block_size, MPI_INT, i, i, MPI_COMM_WORLD, &status);
	//		for (int j = 0; j < block_size; ++j) {
	//			myArr[(i * (N / p)) + j] = receivedelements[j];
	//		}
	//	}
	//	for (int i = 0; i < block_size; ++i) {
	//		myArr[i] = localelements[i];
	//	}

	//}

	MPI_Gather(localelements, block_size, MPI_INT, myArr, block_size, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("Final elements are : \n");
		for (int i = 0; i < N; ++i) {
			printf("%d\t", myArr[i]);
		}
	}

	free(localelements);
	free(receivedelements);

	MPI_Finalize();

	return 0;

}