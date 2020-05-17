/* gram_schmidt.c -- process to obtain a set of orthonormal
 * 				     vectors from a set of linearly independent
 * 					 vectors.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Fri May 15 15:48:10 EEST 2020
 * Copyright: (C) 2020 Luis Colorado.  All rights reserved.
 * License: BSD.
 */

#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"

#define FLAG_DEBUG			(1 << 0)

#define EPSILON		1.0E-10

#define F(_fmt) __FILE__":%d:%s: "_fmt,__LINE__,__func__

/* subtract to row2 k times row1 */
static void P(double **A, int cols, int row1, double k, int row2)
{
	int c; /* k lo hemos usado en la lista de parametros */

	for (c = 0; c < cols; c++) {
		A[row2][c] -= k * A[row1][c];
	} /* for */
} /* P */

static double
DOT(double *V, double *W, int len)
{
	double res = 0.0;	
	while (len--) 
		res += *V++ * *W++;
	return res;
}

void
N(double *V, int len)
{
	double norm = sqrt(DOT(V, V, len));
	int i;
	for (i = 0; i < len; ++i)
		V[i] /= norm;
}


void gram_schmidt(matriz mat, int rows, int cols)
{
	int row2;

	N(mat[0], cols);
	for (row2 = 1; row2 < rows; ++row2) {
		int row1;
		for (row1 = 0; row1 < row2; ++row1) {
			P(mat, cols, row1,
				DOT(mat[row1], mat[row2], cols),
				row2);
		}
		N(mat[row2], cols);
	}
}

int   flags       = 0,
	  random_flag = 0;
char *format      = "%10lg";

int
main(
		int argc,
		char **argv)
{
	matriz A;
	int    rows,
		   cols,
		   opt;
	double det;

	while((opt = getopt(argc, argv, "df:r")) != EOF) {
		switch(opt) {
		case 'd': flags ^= FLAG_DEBUG; break;
		case 'f': format = optarg; break;
		case 'r': random_flag ^= 1; break;
		} /* switch */
	} /* while */

	rows = leer_entero("Rows",    1, INT_MAX);
	cols = leer_entero("Columns", 1, INT_MAX);

	if (random_flag) {
		int r, c;
		A = new_matriz(rows, cols);
		for (r = 0; r < rows; r++)
			for (c = 0; c < cols; c++)
				A[r][c] = (double)rand()/RAND_MAX;
	} else {
		A = leer_matriz(rows, cols, "A");
	}

	printf(F("Matrix A read is:\n"));
	imprime_matriz(A, format, rows, cols, EPSILON);

	gram_schmidt(A, rows, cols);

	printf(F("After applying Gram/Schmidt algorithm is:\n"));
	imprime_matriz(A, format, rows, cols, EPSILON);

	return 0;
} /* main */
