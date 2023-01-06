#include "iostream"
#include "stdlib.h"
#include "mpi.h" 
#include "vector"
#include "set"
using namespace std;

bool swap(int index1, int index2, int * myArr) {
	if (myArr[index2] < myArr[index1]) {
		int temp = myArr[index1];
		myArr[index1] = myArr[index2];
		myArr[index2] = temp;
		return true;
	}
	return false;
}
int compare(const void* a, const void* b)
{
	const int* x = (int*)a;
	const int* y = (int*)b;

	if (*x > *y)
		return 1;
	else if (*x < *y)
		return -1;

	return 0;
}
void swapping(int& a, int& b) {      //swap the content of a and b
	int temp;
	temp = a;
	a = b;
	b = temp;
}
void display(int* array, int size) {
	for (int i = 0; i < size; i++)
		cout << array[i] << " ";
	cout << endl;
}
void bubbleSort(int* array, int size) {
	for (int i = 0; i < size; i++) {
		int swaps = 0;         //flag to detect any swap is there or not
		for (int j = 0; j < size - i - 1; j++) {
			if (array[j] > array[j + 1]) {       //when the current item is bigger than next
				swapping(array[j], array[j + 1]);
				swaps = 1;    //set swap flag
			}
		}
		if (!swaps)
			break;       // No swap in this pass, so array is sorted
	}
}

void compare_split(int* elmnts, int nlocal, int* relements, int nlocal2, bool small, int rank, int rank2) {
	if (rank2 == -1) //if we don't do this then we might encounter a bug! Not mentioned in the textbook! :)) 
		return;
	int* temporarySpace = new int[nlocal];
	int* finalSpace = new int[nlocal];
	for (int i = 0; i < nlocal; ++i) {
		temporarySpace[i] = elmnts[i];
	}
	/*** debug lines/
	//if (rank == 0) {
	//	cout << "Arr1: \t";
	//	for (int i = 0; i < nlocal; ++i) {
	//		cout << elmnts[i] << "\t";
	//	}
	//	cout << "\n";
	//	cout << "Arr2: \t";
	//	for (int i = 0; i < nlocal; ++i) {
	//		cout << relements[i] << "\t";
	//	}
	//	cout << "\n";
	//}
	//
	/***/
	int i = 0, j = 0, k = 0;
	if (small) {
		for (; i < nlocal; ++i) {
			if (j == nlocal || ((k < nlocal) && elmnts[k] <= relements[j])) {
				finalSpace[i] = elmnts[k++];
			}
			else
				finalSpace[i] = relements[j++];
		}
	}
	else {
		for (i = k = j = nlocal - 1; i >= 0; --i) {
			if (j == -1 || (k != -1 && elmnts[k] >= relements[j])) {
				finalSpace[i] = elmnts[k--];
			}
			else
				finalSpace[i] = relements[j--];
		}
	}
	/**overwrite to orignal array**/
 		for (i = 0; i < nlocal; ++i) {
				elmnts[i] = finalSpace[i];
		}
	 
 
	delete[] temporarySpace; 
	delete[] finalSpace; 
}
int main(int arg, char** argv) {
	MPI_Init(&arg, &argv);
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int N = 16;

	//int nEvenPairs = floor(N / 2);
	//int nOddPairs = floor((N - 1) / 2);

	int myArr[N] = { 7, 9, 11, 22, 41, 102, 4, 16, 19, 33, 39, 45, 49 , 88, 91, 92};

	/**program begins here**/
 
	int elementsPerProcess = N / size; 
	int firstIndex = rank * elementsPerProcess;
	int lastIndex = (rank + 1) * elementsPerProcess - 1; //this assumes that all the elements are uniformally divisible on all procs
	//Perhaps, I'd like to account for the elements in the last process as a special case
	int* localElements = new int[elementsPerProcess];
	int* receivedElements = new int[elementsPerProcess];
	
	int k = 0; 
	for (int i = firstIndex; i <= lastIndex; ++i) {
		localElements[k++] = myArr[i]; 
	}

	bubbleSort(localElements, elementsPerProcess);
	 
	int even; 
	int odd; 
	
	if (!(rank % 2)) {
		even = rank + 1; 
		odd = rank - 1;
	} 
	else {
		odd = rank + 1; 
		even = rank - 1;
	}

	if (odd == -1 || odd == elementsPerProcess) {
		odd = MPI_PROC_NULL;
	}
	if (even == -1 || even == elementsPerProcess) {
		even = MPI_PROC_NULL;
	}

	MPI_Status status; 
	for (int i = 0; i < size - 1; ++i) {
		if (!(i % 2)) { //even 
			cout << "From p#" << rank << " to " << even << "\n";
			MPI_Sendrecv(localElements, elementsPerProcess, MPI_INT, even, 0, receivedElements, elementsPerProcess, MPI_INT, even, 0, MPI_COMM_WORLD, &status);
		}
		else {
			cout << "From p#" << rank << " to " << odd << "\n";
			MPI_Sendrecv(localElements, elementsPerProcess, MPI_INT, odd, 0, receivedElements, elementsPerProcess, MPI_INT, odd, 0, MPI_COMM_WORLD, &status);
		}

		compare_split(localElements, elementsPerProcess, receivedElements, elementsPerProcess, (rank < status.MPI_SOURCE), rank, status.MPI_SOURCE);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	//cout << "From proc#" << rank << "\t my elements are : \n";

	//for (int i = 0; i < elementsPerProcess; ++i) {
	//	cout << localElements[i] << "\t";
	//}
	cout << "\n";
	MPI_Barrier(MPI_COMM_WORLD);
 
	/**MERGING**/
	if (rank != 0) {
		MPI_Send(localElements, elementsPerProcess, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else {
		for (int i = 0; i < elementsPerProcess; ++i) {
			myArr[i] = localElements[i];
		}
		for (int i = 1; i < size; ++i) {
			int k = 0;
			MPI_Recv(receivedElements, elementsPerProcess, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			for (int j = (status.MPI_SOURCE * elementsPerProcess); j < ((status.MPI_SOURCE + 1) * elementsPerProcess); j++) {
				myArr[j] = receivedElements[k++];
			}
		}
		cout << "FINAL ARRAY: ";
		for (int i = 0; i < N; ++i) {
			cout << myArr[i]<<"\t";
		}
	}
	delete[] localElements; 
	delete[] receivedElements;
	MPI_Finalize();
		return 0;
	
}
