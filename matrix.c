/* $Id: matrix.c,v 1.1 2014/03/10 09:22:27 luis Exp $
 * vim: ts=4 sw=4 nowrap
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Sat Feb 10 20:51:36 CET 2007
 *
 * Disclaimer:
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#define IN_MATRIX_C

/* Standard include files */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

#define N	5

matriz new_matriz(int l, int c)
{
	matriz res;
	double *p;
	int i;

	assert(res = malloc(
				  sizeof(struct matriz)
				+ l*sizeof (double *)
				+ l*c * sizeof(double)));
	res->l = l; /* lines */
	res->c = c; /* columns */
	res->v = (double **) (res + 1); /* array of pointers to lines */
	p = (double *)(res->v + l); /* array of pointers to cells */
	for (i = 0; i < l; i++) {
		res->v[i] = p;
		p += c;
	} /* for */
	return res;
} /* new_matriz */
	

matriz leer_matriz(char *n, int lin, int col)
{
	matriz res = new_matriz(lin, col);

	for (lin = 0; lin < res->l; lin++) {
		for (col = 0; col < res->c; col++) {
			printf("%s[%d,%d]? ", n, lin, col);
			scanf("%lg", &(res->v[lin][col]));
		} /* for */
	}
} /* leer_matriz */

int esCero(double x)
{
	return fabs(x) < 1.0E-8;
} /* esCero */

void imprime_matriz(matriz m, int nlin, int ncol)
{
	int lin, col;
	for (lin = 0; lin < nlin; lin = lin + 1) {
		for (col = 0; col < ncol; col = col + 1) {
			if (esCero(m[lin][col]))
				printf("\t0");
			else
				printf("\t%lg", m[lin][col]);
		}
		printf("\n");
	}
	printf("\n");
} /* imprime_matriz */

void copia_matriz(double a[][], int nlin, int ncol, double b[][])
{
	int i, j;
	for (i = 0; i < nlin; i = i + 1)
		for (j = 0; j < ncol; j = j + 1)
			b[i][j] = a[i][j];
} /* copia_matriz */

void multiplica_matriz(double a[N][N], double b[N][N], double c[N][N])
{
	int i, j, k;
	for (i = 0; i < N; i = i + 1) {
		for (j = 0; j < N; j = j + 1) {
			c[i][j] = 0.0;
			for (k = 0; k < N; k = k + 1) {
				c[i][j] = c[i][j] + a[i][k] * b[k][j];
			}
		}
	}
} /* multiplica_matriz */

void matriz_identidad(double a[N][N])
{
	int i, j;
	for (i = 0; i < N; i = i + 1) {
		for (j = 0; j < N; j = j + 1) {
			if (i == j) a[i][j] = 1.0;
			else a[i][j] = 0.0;
		} /* for j */
	} /* for i */
} /* matriz_identidad */

void P(double A[N][N], int i, double k, int j)
{
	int l; /* k lo hemos usado en la lista de parametros */
	for (l = 0; l < N; l = l + 1) {
		A[i][l] = A[i][l] + k * A[j][l];
	} /* for */
} /* P */

/* intercambia las filas i y j de la matriz
 * A */
void intercambia(double A[N][N], int i, int j)
{
	int k;
	for (k = 0; k < N; k = k + 1) {
		double aux;
		aux = A[i][k];
		A[i][k] = A[j][k];
		A[j][k] = aux;
	} /* for */
} /* intercambia */

void divide(double A[N][N], int i, double x)
{
	int j;
	for (j = 0; j < N; j = j + 1) {
		A[i][j] = A[i][j] / x;
	} /* for */
} /* divide */

double invertir_matriz(double C[N][N], double B[N][N])
{
	double A[N][N];
	int i, j, k;
	double det = 1.0;

	/* empezamos haciendo una copia de C */
	copia_matriz(C, A);

	/* metemos en B la matriz identidad */
	matriz_identidad(B);

	/* vamos desde la primera fila hasta la
	 * penultima, haciendo ceros en las filas
	 * de debajo de la que estamos
	 * considerando */
	for(i = 0; i < N-1; i = i + 1) {
		/* si el elemento de la diagonal es
		 * cero, no podemos hacer ceros
		 * (hariamos una division por cero)
		 * asi que podemos buscar el elemento
		 * de mayor valor absoluto, debajo del
		 * elemento de la diagonal e
		 * intercambiar las filas. */
		double max = fabs(A[i][i]);
		int i_max = -1;
		for (j = i + 1; j < N; j = j  + 1) {
			if (fabs(A[j][i]) > max) {
				i_max = j;
				max = fabs(A[j][i]);
			}
		}
		/* si i_max es distinto de -1, es que
		 * hay un elemento debajo de la
		 * diagonal principal que tiene valor
		 * absoluto mayor que el elemento de
		 * la diagonal, intercambiamos las
		 * filas (aqui y en la matriz inversa)
		 * y cambiamos de signo el
		 * determinante. */
		if (i_max != -1) {
			intercambia(A, i, i_max);
			intercambia(B, i, i_max);
			det = -det;
		}
		if (esCero(A[i][i])) {
			printf("La matriz A es singular:\n");
			imprime_matriz(A);
			exit(0);
		} /* if */
		/* bien, hacemos ceros debajo de la
		 * diagonal */
		for (j = i + 1; j < N; j = j + 1) {
			double x = A[j][i] / A[i][i];
			/* a la fila j, le quitamos (o
			 * a~nadimos) -x por la fila i */
			P(A, j, -x, i);
			/* tambien lo hacemos  a la matriz
			 * identidad */
			P(B, j, -x, i);
		}
		/* hemos hecho ceros debajo de la
		 * diagonal */
		/* vamos calculando el determinante */
		det = det * A[i][i];
	}
	/* cuando llegamos aqui hemos hecho ceros
	 * debajo de la diagonal, pero el ultimo
	 * elemento podria ser cero.  Sacamos el
	 * determinante. */
	det = det * A[N-1][N-1]; /* recorrimos el bucle
							hasta la penultima fila */
	if (esCero(det)) {
		printf("La matriz A es singular: det = 0.0\n");
		exit(0);
	}

	printf("El determinante vale: %lg\n", det);

	for (i = N-1; i > 0; i = i - 1) {
		for (j = i - 1; j >= 0; j = j - 1) {
			double x = A[j][i] / A[i][i];
			P(A, j, -x, i);
			/* y tambien en la matriz inversa
			 * */
			P(B, j, -x, i);
		}
	}

	for (i = 0; i < N; i = i + 1) {
		double x = A[i][i];
		divide(A, i, x);
		divide(B, i, x);
	}
} /* invertir_matriz */

int main()
{
	double A[N][N], B[N][N], C[N][N];

	printf("Introduzca la matriz A:\n");
	leer_matriz(A, "A");

	printf("La matriz A queda:\n");
	imprime_matriz(A);

	invertir_matriz(A, B);

	printf("La matriz B queda:\n");
	imprime_matriz(B);

	printf("A*B queda:\n");
	multiplica_matriz(A, B, C);
	imprime_matriz(C);

} /* main */

/* $Id: matrix.c,v 1.1 2014/03/10 09:22:27 luis Exp $ */
