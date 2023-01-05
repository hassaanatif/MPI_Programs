#include "omp.h"
#include "stdio.h"

int main(int argc, char** argv) {

	int nthreads, tid;
	const int N = 10;
	int myArr[10] = { 1, 1, 2, 3, 1, 4, 6,9,1,1 };

	int key = 1;
	int count = 0;
#pragma omp parallel for num_threads(4) reduction(+:count)
	for (int i = 0; i < 10; ++i) {
		if (myArr[i] == key) { 
			count++;
		}
	}


printf("Total number of occurance of %d is %d", key, count);


}