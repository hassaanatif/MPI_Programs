#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    const int N = 10;

    bool is_prime[N];

    // Initialize all the numbers to be prime
    for (int i = 0; i < N; ++i) {
        is_prime[i] = true;
    }

    // Sieve of Eratosthenes algorithm
int j; 
#pragma omp parallel for num_threads(10) 
    for (int i = 2; i < N; ++i) {
            int tid = omp_get_thread_num();
             printf("Hello from thread %d\n", tid);
        if (is_prime[i]) {
            // Mark all the multiples of i as not prime
            for (j = i * i; j < N; j += i) {
                is_prime[j] = false;
            }
        }
    }

    // Print the prime numbers
    for (int i = 2; i < N; ++i) {
        if (is_prime[i]) {
            printf("%d is prime\n", i);
        }
    }

    return 0;
}