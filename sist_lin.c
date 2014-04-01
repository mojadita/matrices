/* $Id: sist_lin.c,v 1.2 2014/04/01 17:22:44 luis Exp $
 * vim: ts=4 sw=4 nowrap
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Mon Feb 17 19:51:53 CET 2014
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.  All
 * rights reserved.
 */

/* Standard include files */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#include "io.h"
#include "gauss.h"

#define EPSILON		1.0E-10

int debug = 0, random_flag = 0;

int main(int argc, char **argv)
{
	matriz A;
	int A_l, A_c, opt;
	double det;

	while((opt = getopt(argc, argv, "dr")) != EOF) {
		switch(opt) {
		case 'd': debug ^= 1; break;
		case 'r': random_flag ^= 1; break;
		} /* switch */
	} /* while */

	A_l = leer_entero("Filas", 1, INT_MAX);
	A_c = leer_entero("Columnas", 1, INT_MAX);

	if (random_flag) {
		int i, j;
		A = new_matriz(A_l, A_c);
		for (i = 0; i < A_l; i++)
			for (j = 0; j < A_c; j++)
				A[i][j] = (double)rand()/(double)RAND_MAX;
	} else
		A = leer_matriz(A_l, A_c, "A");

	printf("La matriz A introducida es:\n");
	imprime_matriz(A, A_l, A_c, EPSILON);

	det = gauss(A, A_l, A_c, EPSILON, debug);

	if (!debug) printf("El determinante vale: %lg\n", det);

	printf("La matriz A queda finalmente:\n");
	imprime_matriz(A, A_l, A_c, EPSILON);
} /* main */

/* $Id: sist_lin.c,v 1.2 2014/04/01 17:22:44 luis Exp $ */
