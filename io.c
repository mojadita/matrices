/* $Id: io.c,v 1.1 2014/04/01 17:22:44 luis Exp $
 * vim: ts=4 sw=4 nowrap
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Mon Feb 17 19:51:53 CET 2014
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.  All
 * rights reserved.
 */

/* Standard include files */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

typedef double **matriz;

int leer_entero(char *prompt, int min, int max)
{
	int res;
	bool istty = isatty(0);

	do {
		if (istty) printf("%s? ", prompt);
		scanf("%d", &res);
		if ((res < min) || (res > max)) {
			printf("Ojo, \"%s\" debe estar entre [%d, %d]\n",
					prompt, min, max);
		} /* if */
	} while ((res < min) || (res > max));
	return res;
} /* leer_entero */

matriz new_matriz(int nlin, int ncol)
{
	double **res, *p;
	int i;

	assert(res
		= malloc(nlin*sizeof(double *)
		+ nlin*ncol*sizeof(double)));

	p = (double *)(res + nlin);
	for (i = 0; i < nlin; i++) {
		res[i] = p;
		p += ncol;
	} /* for */

	return res;
} /* new_matriz */

matriz leer_matriz(int nlin, int ncol, char *nombre)
{
	int n, lin, col;
	matriz m;
	bool istty = isatty(0);

	m = new_matriz(nlin, ncol);

	for (lin = 0; lin < nlin; lin++) {
		if (istty) printf("Fila %d\n", lin);
		for (col = 0; col < ncol; col++) {
			if (istty)
				printf("%s[%d,%d] = ",
					nombre,
					lin,
					col);
			scanf("%lg", &m[lin][col]);
		} /* for col */
		if (istty) printf("\n");
	} /* for lin */
	if (istty) printf("\n");
	return m;
} /* leer_matriz */

int
imprime_matriz(
		matriz m,
		int filas,
		int columnas,
		const char *fmt,
		double eps)
{
	int lin, col;
	int res = 0;
	bool istty = isatty(1);

	if (!istty)
		printf("%d %d\n", filas, columnas);
	for (lin = 0; lin < filas; lin++) {
		if (istty)
			res += printf(lin == 0 ? "{{" : " {");
		for (col = 0; col < columnas; col++) {
			if (istty)
				if (col > 0) printf(", ");
			res += printf(fmt, fabs(m[lin][col]) < eps
					? 0.0
					: m[lin][col]);
		}
		if (istty)
			res += printf(lin == filas-1 ? "}}" : "}");
		printf("\n");
	}

	return res;
} /* imprime_matriz */

/* $Id: io.c,v 1.1 2014/04/01 17:22:44 luis Exp $ */
