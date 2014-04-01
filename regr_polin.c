/* $Id: regr_polin.c,v 1.6 2014/04/01 17:22:44 luis Exp $
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

#define DEFAULT_GRADE	1
#define EPSILON		1.0E-10

int debug = 0;

int main(int argc, char **argv)
{
	int opt, i, j, ln = 0;
	int n = DEFAULT_GRADE;
	double *sum_xi, *sum_yi_xi;
	static char line[80];
	matriz A;

	while((opt = getopt(argc, argv, "n:d")) != EOF) {
		switch(opt) {
		case 'd': debug ^= 1; break;
		case 'n': opt = sscanf(optarg, "%d", &n);
			if (opt != 1 || n < 1) {
				fprintf(stderr,
					"WARNING: n parameter invalid(%s), "
					"defaulting to %d\n",
					optarg, DEFAULT_GRADE);
				n = DEFAULT_GRADE;
			} /* if */
			break;
		} /* switch */
	} /* while */

	assert(sum_xi = calloc(2*n + 1, sizeof(double)));
	assert(sum_yi_xi = calloc(n + 1, sizeof(double)));

	for (i = 0; i <= 2*n; i++) sum_xi[i] = 0.0;
	for (i = 0; i <= n; i++) sum_yi_xi[i] = 0.0;

	while (fgets(line, sizeof line, stdin)) {
		double x_i, y_i, x = 1.0;

		ln++;

		switch(sscanf(line, "%lg%lg", &x_i, &y_i)) {
		case 2: break;
		default:
			fprintf(stderr,
				"ERROR: line %d: syntax error.\n",
				ln);
		case 0:
			continue;
		} /* switch */

		for (i = 0; i <= n; i++) {
			sum_xi[i] += x;
			sum_yi_xi[i] += x * y_i;
			x *= x_i;
		} /* for */
		for (; i <= 2*n; i++) {
			sum_xi[i] += x;
			x *= x_i;
		} /* for */
	} /* while */

	assert(A = new_matriz(n+1, n+2));

	for (i = 0; i <= n; i++)
		for (j = 0; j <= n; j++)
			A[i][j] = sum_xi[i+j];

	for (i = 0; i <= n; i++)
		A[i][n+1] = sum_yi_xi[i];

	if (debug) {
		printf("El sistema a resolver es:\n");
		imprime_matriz(A, n+1, n+2, EPSILON);
	} /* if */

	gauss(A, n+1, n+2, EPSILON, debug);

	if (debug) {
		printf("La matriz A queda:\n");
		imprime_matriz(A, n+1, n+2, EPSILON);
	} /* if */

	printf("El polinomio queda:\n");
	
	for (i = 0, j = 0; i <= n; i++) {
		if (fabs(A[i][n+1]) > EPSILON) {
			j++;
			printf(" %+lg", A[i][n+1]);
			if (i) printf("*X");
			if (i > 1) printf("^%d", i);
			if(j % 8 == 0) printf("\n");
		}
	} /* for */
	if (j % 8 != 0) printf("\n");

	return 0;
} /* main */

/* $Id: regr_polin.c,v 1.6 2014/04/01 17:22:44 luis Exp $ */
