#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    const int N = 10;

    int myArr[10] = { 10, 9, 5, 1, 4, 3, 7, 8, 6,2 };

    bool even = true;

 
    for (int i = 0; i < N; ++i) {
       
  
      if (even) {
            
            for (int j = 0; j < N; j+=2) {
                if (myArr[j] > myArr[j + 1]) {
                    int tmp = myArr[j];
                    myArr[j] = myArr[j + 1];
                    myArr[j + 1] = tmp;
                }
            }
         even = false;
        }
        else {
            //odd
       
            for (int j = 1; j < N; j += 2) {
                if (myArr[j] > myArr[j + 1]) {
                    int tmp = myArr[j];
                    myArr[j] = myArr[j + 1];
                    myArr[j + 1] = tmp;
                }
            }
           even = true;
        }
    }

    for (int i = 0; i < N; ++i) {
        printf("%d\t", myArr[i]);
    }
    return 0;
}