// Adam Ovadia
// CSCI 493
// Multithreaded Matrix Multiplication
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "barrier.h"

#define N 3
#define M 5
#define I 3

int m1[N][M];
int m2[M][I];
int m3[N][I];

pthread_barrier_t *barrier;
pthread_barrierattr_t *attr;

pthread_t tid[N*I+1];
void *tret[N];
void *singleMultiplcation(void *ptr);
void printMatrix(int x, int y, int m[x][y]);
void generateRandomMatrix(int x, int y, int m[x][y]);

int main() {
 
    int thread;
    barrier=malloc(sizeof(pthread_barrier_t));
    attr=malloc(sizeof(pthread_barrierattr_t));
    thread = pthread_barrier_init(barrier, attr, N*I);
    
    generateRandomMatrix(N, M, m1);
    printf("Matrix1:\n");
    printMatrix(N, M, m1);
   
    generateRandomMatrix(M, I, m2);

    printf("Matrix2:\n");
    printMatrix(M, I, m2);
    
    printf("Starting Matrix Multiplication...\n");
   
    printf("For the Matrix Multiplicationof %dx%d and %dx%d\n   we can use %d threads!\n ", N, M, I, N, N*I);
    printf("Creating %d threads \n", N*I);
    int c = 0;
    int d = 0;
    
    for (int a = 0; a < N*I; a++) {
            thread = pthread_create(&tid[a], NULL, singleMultiplcation, (void *)c);
        d++;
        if (d % I ==0) {
            c+=N-I+1;
        }
        else {
            c++;
        }
    }
    for (int a = 0; a < N*I+1; a++) {
        thread = pthread_join(tid[a], NULL);

    }
    printf("joined threads back to main thread\n");

    thread = pthread_barrier_destroy(barrier);
    printf("Result Matrix:\n");
    printMatrix(N, I, m3);
}

void *singleMultiplcation(void *ptr) {
    pthread_t tidID;
    tidID=pthread_self();
    printf("Thread %d: tid=%lu, tid(hex)=0x%lx.\n", (int) ptr, (unsigned long) tidID, (unsigned long) tidID);
    
    int pos = (int) ptr;
    
    //printf("I received pos: %d\n", pos);  //debug
    
    int start = pos / N;
    int add = pos % N;
    m3[start][add] = 0;
    for ( int a = 0; a <  N; a++) {
            m3[start][add] += m1[start][a] * m2[a][add];
    }

   // printf("M3: %d\n", m3[start][add]); //debug
    int err=pthread_barrier_wait(barrier);
    sleep(2);
    
    printf("Thread %d completed\n", (int)ptr);
  
    pthread_exit ((void *) pos);
}

void printMatrix(int x, int y, int m[x][y]) {
    for (int a = 0; a < x; a++) {
        for (int b = 0; b < y; b++) {
            printf("%-6d ", m[a][b]);
        }
        printf("\n\n");
    }
}

void generateRandomMatrix(int x, int y, int m[x][y]) {
    for (int a  = 0; a <x; a++) {
        for (int b = 0; b < y; b++) {
            m[a][b] = rand() % 100 +1;
        }
    }
}
