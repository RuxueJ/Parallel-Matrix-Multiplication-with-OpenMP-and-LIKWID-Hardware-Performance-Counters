#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "likwid-stuff.h"

const char* dgemm_desc = "Blocked dgemm, OpenMP-enabled";


/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are n-by-n matrices stored in row-major format.
 * On exit, A and B maintain their input values. */
void square_dgemm_blocked(int n, int block_size, double* A, double* B, double* C) 
{
   // insert your code here: implementation of blocked matrix multiply with copy optimization and OpenMP parallelism enabled

   // be sure to include LIKWID_MARKER_START(MY_MARKER_REGION_NAME) inside the block of parallel code,
   // but before your matrix multiply code, and then include LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME)
   // after the matrix multiply code but before the end of the parallel code block.

   std::cout << "Insert your blocked matrix multiply with copy optimization, openmp-parallel edition here " << std::endl;
   // insert your code here
   int N = n / block_size;
   #pragma omp parallel
   {

       // local storage for each thread
       double* matrixCopyA = new double[block_size * block_size];
       double* matrixCopyB = new double[block_size * block_size];
       double* matrixCopyC = new double[block_size * block_size];

       LIKWID_MARKER_START(MY_MARKER_REGION_NAME);
       #pragma omp for collapse(2)

       for (int i = 0; i < N; i++) {
           for (int j = 0; j < N; j++) {

               // Create a block of C
               //double* matrixCopyC = new double[block_size * block_size];
               int startRowC = i * block_size;
               int startColC = j * block_size;

               // Copy the current block of C
               for (int relativeRow = 0; relativeRow < block_size; relativeRow++) {
                   memcpy(matrixCopyC + relativeRow * block_size, C + (startRowC + relativeRow) * n + startColC, block_size * sizeof(double));
               }

               // Multiply blocks of A and B, accumulate into matrixCopyC
               for (int k = 0; k < N; k++) {

                   // Create a block of A
                   //double* matrixCopyA = new double[block_size * block_size];
                   int startRowA = i * block_size;
                   int startColA = k * block_size;

                   // Copy the current block of A
                   for (int relativeRow = 0; relativeRow < block_size; relativeRow++) {
                       memcpy(matrixCopyA + relativeRow * block_size, A + (startRowA + relativeRow) * n + startColA, block_size * sizeof(double));
                   }

                   // Create a block of B
                   //double* matrixCopyB = new double[block_size * block_size];
                   int startRowB = k * block_size;
                   int startColB = j * block_size;

                   // Copy the current block of B
                   for (int relativeRow = 0; relativeRow < block_size; relativeRow++) {
                       memcpy(matrixCopyB + relativeRow * block_size, B + (startRowB + relativeRow) * n + startColB, block_size * sizeof(double));
                   }


                   // basic matrix multiplication on each bXb block
                   for (int i = 0; i < block_size; ++i) {
                       for (int j = 0; j < block_size; ++j) {
                           for (int k = 0; k < block_size; ++k) {
                               // Perform the multiplication and accumulation
                               matrixCopyC[i * block_size + j] += matrixCopyA[i * block_size + k] * matrixCopyB[k * block_size + j];
                           }
                       }
                   }


               }


               // write the C[i,j] back to memory
               for (int relativeRow = 0; relativeRow < block_size; relativeRow++) {
                   memcpy(C + (startRowC + relativeRow) * n + startColC,
                       matrixCopyC + relativeRow * block_size,
                       block_size * sizeof(double));
               }


           }
       }

       LIKWID_MARKER_STOP(MY_MARKER_REGION_NAME);

       delete[] matrixCopyA;
       delete[] matrixCopyB;
       delete[] matrixCopyC;
   }
}
