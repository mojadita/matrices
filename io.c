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

/* reads an int with prompt if input is a tty.
 * Checks that the value is between min and max,
 * and repeats the question in case it is not. */
int read_int(char *prompt, int min, int max)
{
    int res = 0, n;
    bool istty = isatty(0);

    do {
        if (istty) printf("%s? ", prompt);
        n = scanf("%d", &res);
        if ((res < min) || (res > max)) {
            printf("Ojo, \"%s\" debe estar entre [%d, %d]\n",
                    prompt, min, max);
        } /* if */
    } while (n > 0 && (res < min || res > max));
    return res;
} /* leer_entero */

/* generates a new matrix of nlin lines and
 * ncol columns.
 * The matrix is allocated in one single malloc, storing
 * the pointers of each row at the beginning, then the
 * array, so a matrix can be simulated by a double pointer. */
double **
new_matrix(int nlin, int ncol)
{
    double **res, *p;
    int i;

    res = malloc(nlin*sizeof(double) + nlin*ncol*sizeof(double));
    assert(res != NULL);

    p = (double *)(res + nlin);
    for (i = 0; i < nlin; i++) {
        res[i] = p;
        p += ncol;
    } /* for */

    return res;
} /* new_matrix */

/* reads a matrix of row and cols.
 * Name is used for the prompts. */
double **
read_matrix(int rows, int cols, char *name)
{
    int         row,
                col;
    double**    m     = new_matrix(rows, cols);
    bool        istty = isatty(0);

    for (row = 0; row < rows; row++) {
        if (istty) printf("Row %d\n", row);
        for (col = 0; col < cols; col++) {
			if (istty)
				printf("%s[%d,%d] = ",
					name, row, col);
			scanf("%lg", &m[row][col]);
        } /* for col */
        if (istty) printf("\n");
    } /* for lin */
    if (istty) printf("\n");

    return m;
} /* read_matrix */

int
print_matrix(
        double **       m,
        int             rows,
        int             cols,
        const char *    fmt,
        double          eps)
{
    int     row,
            col;
    int     res = 0;
    bool  istty = isatty(1);

    if (!istty)
        printf("%d %d\n", rows, cols);
    for (row = 0; row < rows; row++) {
        if (istty)
            res += printf(row == 0
                    ? "{{"
                    : " {");
        for (col = 0; col < cols; col++) {
            if (istty && col > 0)
                    printf(", ");
            res += printf(fmt,
                    fabs(m[row][col]) < eps
                        ? 0.0
                        : m[row][col]);
        }
        if (istty)
            res += printf(
                    row == rows-1
                        ? "}}"
                        : "}");
        printf("\n");
    }

    return res;
} /* print_matrix */

/* $Id: io.c,v 1.1 2014/04/01 17:22:44 luis Exp $ */
