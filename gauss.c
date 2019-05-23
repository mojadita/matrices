/* $Id: gauss.c,v 1.10 2014/04/01 17:22:44 luis Exp $
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

#ifndef FALSE
#define	FALSE	0
#define TRUE	(!FALSE)
#endif

/* suma a la fila i-esima de la matriz A (de cols columnas)
 * la fila j-esima multiplicada por k. */
static void P(double **A, int cols, int i, double k, int j)
{
	int l; /* k lo hemos usado en la lista de parametros */

	for (l = 0; l < cols; l++) {
		A[i][l] = A[i][l] + k * A[j][l];
	} /* for */
} /* P */

/* intercambia las filas i y j de la matriz A */
static void intercambia(double **A, int cols, int i, int j)
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
static void divide_fila(double **A, int cols, int i, double x)
{
	int j;
	for (j = 0; j < cols; j++) {
		A[i][j] /= x;
	} /* for */
} /* divide_fila */

/* Realiza el metodo de eliminacion de Gauss a una matriz de
 * lin lineas y col columnas. */
double gauss(double **A, int lin, int col, double eps, int debug)
{
	int i, j, k;
	double det = 1.0;

	/* vamos desde la primera fila hasta la
	 * penultima, haciendo ceros en las filas
	 * de debajo del pivote (el pivote es el
	 * elemento de la fila i, columna j. */
	for(i = 0, j = 0; i < lin && j < col; i++, j++) {
		int mod = FALSE;
		if (fabs(A[i][j]) < eps) {
			/* El pivote tiene que ser distinto de
			 * cero, no podemos hacer ceros
			 * (hariamos una division por cero)
			 * debajo de la diagonal.
			 * Asi que buscamos el elemento
			 * de mayor valor absoluto, debajo del
			 * pivote e intercambiamos filas. */
			double max = fabs(A[i][j]);
			int i_max = i;
			for (k = i + 1; k < lin; k++) {
				if (fabs(A[k][j]) > max) {
					i_max = k;
					max = fabs(A[k][j]);
				} /* if */
			} /* for j */
			/* si i_max es distinto de i, es que
			 * hay un elemento debajo de la
			 * diagonal principal que tiene valor
			 * absoluto mayor que el elemento de
			 * la diagonal, intercambiamos las
			 * filas y cambiamos de signo el
			 * determinante. */
			if (i_max != i) {
				intercambia(A, col, i, i_max);
				det = -det;
				mod = TRUE;
			}
		} /* if */

		/* vamos calculando el determinante siempre
		 * que los pivotes esten en la diagonal. */
		if (i == j) det *= A[i][j];

		/* si el candidato a pivote sigue siendo 
		 * demasiado peque~no, no podemos hacer
		 * ceros, ya que dividimos por un numero
		 * demasiado peque~no. Esto significa que
		 * todos los elementos debajo del pivote son
		 * cero y que no hay pivote, hemos de pasar
		 * al elemento de la derecha. */
		if (fabs(A[i][j]) < eps) {
			if (i == j) {
				/* lo imprimimos solo la primera vez. */
				printf("La matriz A es singular.\n");
			}
			/* cuando se incrementen i y j, el
			 * elemento seleccionado como pivote
			 * sera el elemento a la derecha del
			 * ex-pivote actual. */
			i--; 
			continue;
		} /* if */

		/* bien, hacemos ceros debajo del
		 * pivote */
		for (k = i + 1; k < lin; k++) {
			if (1 || fabs(A[k][j]) >= eps) {
				double x = -A[k][j] / A[i][j];
				/* a la fila k, le a~nadimos)
				 * x por la fila i */
				P(A, col, k, x, i);
				mod = TRUE;
			} /* if */
		} /* for */
		/* hemos hecho ceros debajo de los
		 * pivotes. */
		if (debug && mod) imprime_matriz(A, lin, col, eps);
	} /* for i */

	if (debug)
		printf("El determinante vale: %lg\n",
			(i == j) ? det : 0.0);

	/* si col > lin es que la matriz se ha introducido como
	 * un sistema.  En este caso, debemos continuar, hacer
	 * ceros sobre la diagonal y obtener una matriz
	 * identidad, de forma que se resuelva el sistema de
	 * ecuaciones planteado. */
	if (col > lin) {
		for (i = lin-1; i >= 0; i--) {
			int mod = FALSE;
			double p;
			j = i;
			if (fabs(A[i][j]) < eps) {
				/* buscamos el pivote.  Este sera
				 * el primer elemento distinto de cero
				 * (en valor absoluto, mayor que epsilon)
				 * que haya a la derecha del elemento de
				 * la diagonal */
				for (j = i + 1; j < col; j++) {
					if (fabs(A[i][j]) >= eps)
						break;
				}
			} /* if */

			/* si hemos encontrado el pivote... */
			if (j < col) {

				/* A[i][j] es el pivote */
				p = A[i][j];
				/* hacemos ceros sobre el pivote */
				for (k = i - 1; k >= 0; k--) {
					if (1||fabs(A[k][j]) >= eps) {
						double x = -A[k][j] / A[i][j];
						P(A, col, k, x, i);
						mod = TRUE;
					} /* if */
				} /* for k */

				/* dividimos toda la fila por el pivote. */
				if (fabs(p - 1.0) >= eps) 
					divide_fila(A, col, i, p);
			} /* if */
			if (debug && mod) imprime_matriz(A, lin, col, eps);
		} /* for i */
	} /* if  */

	return det;
} /* gauss */

/* $Id: gauss.c,v 1.10 2014/04/01 17:22:44 luis Exp $ */
