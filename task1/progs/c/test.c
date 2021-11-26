#include <stdlib.h>
#include <stdio.h>

/* DGESV prototype */
extern void dgesv_( int* n, int* nrhs, double* a, int* lda, int* ipiv,
                double* b, int* ldb, int* info );
/* Auxiliary routines prototypes */
extern void print_matrix( int m, int n, double* a, int lda );
extern void print_int_vector( int n, int* a );

/* Parameters */
#define N 5
#define NRHS 3
#define LDA N
#define LDB N

/* Main program */
int main(void) {
        /* Locals */
        int n = N, nrhs = NRHS, lda = LDA, ldb = LDB, info;
        /* Local arrays */
        int ipiv[N];
        double a[LDA*N] = {
            6.80, -2.11,  5.66,  5.97,  8.23,
           -6.05, -3.30,  5.36, -4.44,  1.08,
           -0.45,  2.58, -2.70,  0.27,  9.04,
            8.32,  2.71,  4.35, -7.17,  2.14,
           -9.67, -5.14, -7.26,  6.08, -6.87
        };
        double b[LDB*NRHS] = {
            4.02,  6.19, -8.22, -7.57, -3.03,
           -1.56,  4.00, -8.67,  1.75,  2.86,
            9.81, -4.09, -4.57, -8.61,  8.99
        };
        /* Executable statements */
        printf( " DGESV Example Program Results\n" );
        /* Solve the equations A*X = B */
        dgesv_( &n, &nrhs, a, &lda, ipiv, b, &ldb, &info );
        /* Check for the exact singularity */
        if( info > 0 ) {
                printf( "The diagonal element of the triangular factor of A,\n" );
                printf( "U(%i,%i) is zero, so that A is singular;\n", info, info );
                printf( "the solution could not be computed.\n" );
                exit( 1 );
        }
        /* Print solution */
        print_matrix( n, nrhs, b, ldb );
        /* Print details of LU factorization */
        print_matrix(  n, n, a, lda );
        /* Print pivot indices */
        print_int_vector( n, ipiv );
        exit( 0 );
} /* End of DGESV Example */

/* Auxiliary routine: printing a matrix */
void print_matrix( int m, int n, double* a, int lda ) {
        int i, j;
        for( i = 0; i < m; i++ ) {
                for( j = 0; j < n; j++ ) printf( " %6.2f", a[i+j*lda] );
                printf( "\n" );
        }
}

/* Auxiliary routine: printing a vector of integers */
void print_int_vector(  int n, int* a ) {
        int j;
        for( j = 0; j < n; j++ ) printf( " %6i", a[j] );
        printf( "\n" );
}