/* $Id: gauss.c,v 1.10 2014/04/01 17:22:44 luis Exp $
 * vim: ts=4 sw=4 nowrap
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Mon Feb 17 19:51:53 CET 2014
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.  All
 * rights reserved.
 */

/* Standard include files */
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "io.h"
#include "gauss.h"

#include "io.h"

#ifndef FALSE
#define	FALSE	0
#define TRUE	(!FALSE)
#endif

#define F(_fmt) __FILE__":%d:%s: " _fmt, __LINE__, __func__

#define MIN(_a, _b) ((_a) < (_b) ? (_a) : (_b))

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
static void I(double **A, int cols, int i, int j)
{
	double *aux = A[i];
	A[i] = A[j];
	A[j] = aux;
} /* I */

/* Divide los elementos de la fila i por el valor x. */
static void D(double **A, int cols, int i, double x)
{
	int j;
	for (j = 0; j < cols; j++) {
		A[i][j] /= x;
	} /* for */
} /* D */

/* Realiza el metodo de eliminacion de Gauss a una matriz de
 * rows lineas y col columnas. */
double
gauss(
		double **A,
		int rows,
		int cols,
		double eps,
		int flags,
		const char *fmt)
{
	int pr, pc;
	double det = 1.0;
	int range = MIN(rows, cols);

	if (!fmt) fmt = "%lg";

	/* we declare pivots as const, because it cannot move (as we
	 * have to free(pivots) at the end. */
	struct pivot {
		int row, col;
		double val;
	} *const pivots = malloc(range * sizeof *pivots);

	assert(pivots != NULL);

	/* go from first pivot to previous to last, making zeros
	 * under the pivot.  Pivot is initialized to (0,0) and allows
	 * to determine the rank of the matrix. */
	struct pivot *p = pivots;
	for(pr = 0, pc = 0; pr < rows && pc < cols; pr++, pc++) {
		bool mod = FALSE;
		p->row = pr;
		p->col = pc;
		double pv = A[pr][pc];
		p->val = pv;

		/* if the pivot candidate is not valid, search for a
		 * better one. */
		if (fabs(pv) < eps) {
			/* The pivot must be != zero, as we use it in the
			 * denominator of the algorithm that makes zeros under
			 * (and above) the diagonal.
			 * We must search under it for an suitable element,
			 * and exchange rows, so the pivot becomes that
			 * element.
			 * We search for the greatest value (in absolute
			 * value) in order to get the lowest value to
			 * subtract. */
			double max = fabs(pv);
			int r_max = pr;
			int r;

			for (r = pr + 1; r < rows; r++) {
				if (fabs(A[r][pc]) > max) {
					r_max = r;
					max = fabs(A[r][pc]);
				}
			}

			/* if r_max is different than pr, we found a larger
			 * value to use as pivot.  Just update and restart.
			 */
			if (r_max > pr) {
				/* exchange rows pr and r_max */
				I(A, cols, pr, r_max);

				/* in order to return a correct value */
				det = -det;

				/* the pivot value changed */
				pv = A[pr][pc];
				/* but pivot position didn't */

				mod = TRUE;
			}
		} /* if (fabs(pv) < eps) */

		/* determinant calculus.  We do this only if the pivot
		 * is on the diagonal, as if we departed from it, the
		 * determinant is zero. */
		if (pr == pc) {
			det *= A[pr][pc];
		}

		/* If the pivot candidate continues to be too small, we
		 * we cannot continue zeroing, as we have a singular
		 * pivot.
		 * This means that all the elements under the pivot are
		 * smaller than it (all to be considered zero), so we
		 * have a one less rank matrix.
		 * We need to continue finding pivots on
		 * the right element. */
		if (fabs(pv) < eps) {
			/* decrement range, row (so we go next to the element
			 * to the right of this one), and continue to next
			 * pivot candidate. */
			range--;
			pr--; 
			continue;
		}

		/* If we got here, we have a valid pivot, let's zero the
		 * column under it. */
		int r;
		for (r = pr + 1; r < rows; r++) {
			double x = -A[r][pc] / pv;
			/* add x times row pr to row r */
			P(A, cols, r, x, pr);
			mod = TRUE;
		}

		/* print the pivot position and value if debug */
		if (flags & (FLAG_DEBUG | FLAG_PRINTPIVOTS)) {
			printf(F("Pivot %ld at [r=%d, c=%d]: val=%lg\n"),
				(long)(p-pivots), pr, pc, pv);
		}

        /* now we have zeros under the pivots */
		if ((flags & FLAG_DEBUG) && mod) {
			imprime_matriz(A, fmt, rows, cols, eps);
		}

		/* add the pivot to the stack */
		p++;
	} /* for pr */

	if (flags & FLAG_DEBUG) {
		printf(F("The determinant is %lg\n"), det);
		printf(F("Range of matrix is %d\n"), range);
	}

	/* Now we go backwards, using the pivots in the array,
	 * zeroing the matrix above them, until we get a
	 * quasi-diagonal mattrix. */
	if (!(flags & FLAG_DONTDOLASTPASS)) {
		while (--p >= pivots) {
			bool mod = FALSE;

			/* pivot position */
			int pr = p->row,
				pc = p->col;
			double pv = p->val;

			if (flags & FLAG_DEBUG) {
				printf(F("Processing pivot %ld at "
						"[r=%d][c=%d] (val = %lg)\n"),
					(long)(p - pivots), pr, pc, pv);
			}

			/* zero above the pivot */
            int r;
			for (r = 0; r < pr; r++) {
				double k = -A[r][pc] / pv;
				P(A, cols, r, k, pr);
				mod = TRUE;
			} /* for r */

			/* divide all the row by the pivot */
			if (fabs(pv - 1.0) >= eps) {
				D(A, cols, pr, pv);
				mod = TRUE;
			}
			if ((flags & FLAG_DEBUG) && mod) {
				imprime_matriz(A, fmt, rows, cols, eps);
			}
		} /* while more pivots */
	} /* if (flags & FLAG_LASTPASS) */

	free(pivots);

	return det;
} /* gauss */

/* $Id: gauss.c,v 1.10 2014/04/01 17:22:44 luis Exp $ */
