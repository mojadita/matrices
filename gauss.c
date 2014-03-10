/* $Id: gauss.c,v 1.1.1.1 2014/03/10 09:22:27 luis Exp $
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

#define EPSILON		1.0E-10

int debug = 0;

typedef double **matriz;

static int leer_entero(char *prompt, int min, int max)
{
	int res;

	do {
		if (isatty(0)) printf("%s? ", prompt);
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

	m = new_matriz(nlin, ncol);

	for (lin = 0; lin < nlin; lin++) {
		if (isatty(0)) printf("Fila %d\n", lin);
		for (col = 0; col < ncol; col++) {
			if (isatty(0))
				printf("%s[%d][%d] = ",
					nombre,
					lin,
					col);
			scanf("%lg", &m[lin][col]);
		} /* for col */
		if (isatty(0)) printf("\n");
	} /* for lin */
	if (isatty(0)) printf("\n");
	return m;
} /* leer_matriz */

int esCero(double x)
{
	return fabs(x) < EPSILON;
} /* esCero */

int imprime_matriz(matriz m, int filas, int columnas)
{
	int lin, col;
	int res = 0;

	for (lin = 0; lin < filas; lin++) {
		res += printf(lin == 0 ? "{{" : " {");
		for (col = 0; col < columnas; col++) {
			if (col > 0) printf(",\t");
			if (esCero(m[lin][col]))
				res += printf("0");
			else
				res += printf("%lg", m[lin][col]);
		}
		res += printf(lin == filas-1 ? "}}\n" : "}\n");
	}
	res += printf("\n");

	return res;
} /* imprime_matriz */

/* suma a la fila i-esima de la matriz A (de cols columnas)
 * la fila j-esima multiplicada por k. */
static void P(matriz A, int cols, int i, double k, int j)
{
	int l; /* k lo hemos usado en la lista de parametros */

	for (l = 0; l < cols; l++) {
		A[i][l] = A[i][l] + k * A[j][l];
	} /* for */
} /* P */

/* intercambia las filas i y j de la matriz A */
static void intercambia(matriz A, int cols, int i, int j)
{
	int k;
	for (k = 0; k < cols; k++) {
		double aux;
		aux = A[i][k];
		A[i][k] = A[j][k];
		A[j][k] = aux;
	} /* for */
} /* intercambia */

/* Divide los elementos de la fila i por el valor x. */
static void divide_fila(matriz A, int cols, int i, double x)
{
	int j;
	for (j = 0; j < cols; j++) {
		A[i][j] = A[i][j] / x;
	} /* for */
} /* divide_fila */

/* Realiza el metodo de eliminacion de Gauss a una matriz de
 * lin lineas y col columnas. */
double gauss(matriz A, int lin, int col, int debug)
{
	int i, j, k;
	double det = 1.0;

	/* vamos desde la primera fila hasta la
	 * penultima, haciendo ceros en las filas
	 * de debajo de la que estamos
	 * considerando */
	for(i = 0; i < lin-1; i++) {
		int mod = 0;
		if (esCero(A[i][i])) {
			/* si el elemento de la diagonal es
			 * cero, no podemos hacer ceros
			 * (hariamos una division por cero)
			 * asi que podemos buscar el elemento
			 * de mayor valor absoluto, debajo del
			 * elemento de la diagonal e
			 * intercambiar las filas. */
			double max = fabs(A[i][i]);
			int i_max = i;
			for (j = i + 1; j < lin; j++) {
				if (fabs(A[j][i]) > max) {
					i_max = j;
					max = fabs(A[j][i]);
				} /* if */
			} /* for j */
			/* si i_max es distinto de i, es que
			 * hay un elemento debajo de la
			 * diagonal principal que tiene valor
			 * absoluto mayor que el elemento de
			 * la diagonal, intercambiamos las
			 * filas (aqui y en la matriz inversa)
			 * y cambiamos de signo el
			 * determinante. */
			if (i_max != i) {
				intercambia(A, col, i, i_max);
				det = -det;
				mod = 1;
			}
		} /* if */
		if (esCero(A[i][i])) {
			printf("La matriz A es singular:\n");
			printf("el rango es %d\n", i);
			return 0.0;
		} /* if */
		/* bien, hacemos ceros debajo de la
		 * diagonal */
		for (j = i + 1; j < lin; j++) {
			if (!esCero(A[j][i])) {
				double x = -A[j][i] / A[i][i];
				/* a la fila j, le a~nadimos)
				 * x por la fila i */
				P(A, col, j, x, i);
				mod = 1;
			} /* if */
		} /* for */
		/* hemos hecho ceros debajo de la
		 * diagonal */
		/* vamos calculando el determinante */
		det = det * A[i][i];
		if (debug && mod) imprime_matriz(A, lin, col);
	} /* for i */

	/* cuando llegamos aqui hemos hecho ceros
	 * debajo de la diagonal, pero el ultimo
	 * elemento podria ser cero.  Sacamos el
	 * determinante. */
	det = det * A[i][i];

	if (esCero(det)) {
		printf("La matriz A es singular: det = 0.0\n");
		printf("rango de A es %d\n", lin-1);
		return 0.0;
	}

	if (debug) printf("El determinante vale: %lg\n", det);

	/* si col > lin es que la matriz se ha introducido como
	 * un sistema.  En este caso, debemos continuar, hacer
	 * ceros sobre la diagonal y obtener una matriz
	 * identidad, de forma que se resuelva el sistema de
	 * ecuaciones planteado. */
	if (col > lin) {
		for (i = lin-1; i > 0; i--) {
			int mod = 0;
			for (j = i - 1; j >= 0; j--) {
				if (!esCero(A[j][i])) {
					double x = -A[j][i] / A[i][i];
					P(A, col, j, x, i);
					mod = 1;
				} /* if */
			} /* for j */
			if (debug && mod) imprime_matriz(A, lin, col);
		} /* for i */

		for (i = 0; i < lin; i++) {
			double x = A[i][i];
			divide_fila(A, col, i, x);
		} /* for i */
	}
	return det;
} /* gauss */

int main(int argc, char **argv)
{
	matriz A;
	int A_l, A_c, opt;

	while((opt = getopt(argc, argv, "d")) != EOF) {
		switch(opt) {
		case 'd': debug ^= 1; break;
		} /* switch */
	} /* while */

	A_l = leer_entero("Filas", 1, INT_MAX);
	A_c = leer_entero("Columnas", 1, INT_MAX);
	printf("A_l(%d)*(sizeof(double *)(%d) + A_c(%d) "
		"* sizeof(double)(%d)) = %d\n",
		A_l, sizeof(double *),
		A_c,
		sizeof(double),
		A_l*(sizeof(double *) + A_c*sizeof(double)));
	assert(A_l*((int)sizeof(double *) + A_c * (int)sizeof(double)) > 0);

	A = leer_matriz(A_l, A_c, "A");

	printf("La matriz A introducida es:\n");
	imprime_matriz(A, A_l, A_c);

	gauss(A, A_l, A_c, debug);

	printf("La matriz A queda finalmente:\n");
	imprime_matriz(A, A_l, A_c);
} /* main */

/* $Id: gauss.c,v 1.1.1.1 2014/03/10 09:22:27 luis Exp $ */
