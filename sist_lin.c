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

#define F(_fmt) __FILE__":%d:%s: "_fmt,__LINE__,__func__

#define EPSILON     1.0E-10

int   flags       = 0,
      random_flag = 0;
char *format      = "%10lg";

int
main(
        int         argc,
        char **     argv)
{
    double **       A;
    int             rows,
                    cols,
                    opt;
    double          det;

    while((opt = getopt(argc, argv, "df:r")) != EOF) {
        switch(opt) {
        case 'd': flags ^= FLAG_DEBUG; break;
        case 'f': format = optarg; break;
        case 'r': random_flag ^= 1; break;
        } /* switch */
    } /* while */

    rows = read_int("Rows",    1, INT_MAX);
    cols = read_int("Columns", 1, INT_MAX);

    if (random_flag) {
        int r, c;
        A = new_matrix(rows, cols);
        for (r = 0; r < rows; r++)
            for (c = 0; c < cols; c++)
                A[r][c] = (double)rand()/RAND_MAX;
    } else {
        A = read_matrix(rows, cols, "A");
    }

    printf(F("Matrix A read is:\n"));
    print_matrix(A, rows, cols, format, EPSILON);

    det = gauss(A, rows, cols, EPSILON, flags, format);

    printf(F("Determinant value is: %lg\n"), det);

    printf(F("After applying Gauss/Jordan algorithm is:\n"));
    print_matrix(A, rows, cols, format, EPSILON);

    return 0;
} /* main */

/* $Id: sist_lin.c,v 1.2 2014/04/01 17:22:44 luis Exp $ */
