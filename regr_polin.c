/* $Id: regr_polin.c,v 1.6 2014/04/01 17:22:44 luis Exp $
 * vim: ts=4 sw=4 nowrap
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Mon Feb 17 19:51:53 CET 2014
 * Disclaimer: (C) 2014-2020 LUIS COLORADO SISTEMAS S.L.U.
 *             All rights reserved.
 * License: BSD.
 */

#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>

#include "io.h"
#include "gauss.h"

#define DEFAULT_GRADE   1
#define EPSILON         1.0E-10

int flags = 0;

char *format = "%lg";

void process(FILE *f, int n)
{
    int
            opt,
            i,
            j,
            ln = 0;
    double *sum_xi,
           *sum_yi_xi;
    static char
            line[1000];
    double **
            A;

    assert(sum_xi = calloc(2*n + 1, sizeof(double)));
    assert(sum_yi_xi = calloc(n + 1, sizeof(double)));

    for (i = 0; i <= 2*n; i++) sum_xi[i] = 0.0;
    for (i = 0; i <= n; i++) sum_yi_xi[i] = 0.0;

    while (fgets(line, sizeof line, f)) {
        double      x_i,
                    y_i,
                    x    = 1.0;

        ln++; /* line number */

        switch(sscanf(line, "%lg%lg", &x_i, &y_i)) {
        case 0: continue; /* nothing read */
        case 2: break; /* ok */
        default:
            fprintf(stderr,
                "ERROR: line %d: syntax error.\n",
                ln);
        } /* switch */

        for (i = 0; i <= 2 * n; i++) {
            sum_xi[i]        += x;
            if (i <= n)
                sum_yi_xi[i] += x * y_i;
            x *= x_i;
        } /* for */
    } /* while */

    A = new_matrix(n+1, n+2);

    assert(A != NULL);

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= n; j++)
            A[i][j] = sum_xi[i+j];
        A[i][n+1]   = sum_yi_xi[i];
    }

    if (flags & FLAG_DEBUG) {
        printf("System to solve is:\n");
        print_matrix(A, n+1, n+2, format, EPSILON);
    } /* if */

    gauss(A, n+1, n+2, EPSILON, flags, format);

    if (flags & FLAG_DEBUG) {
        printf("Resultant matrix is:\n");
        print_matrix(A, n+1, n+2, format, EPSILON);
    } /* if */

    if (flags & FLAG_DEBUG) {
        printf("Polynomial is:\n");
    } /* if */

    for (i = 0, j = 0; i <= n; i++) {
        if (fabs(A[i][n+1]) > EPSILON) {
            j++;
            printf(" %+lg", A[i][n+1]);
            if (i) printf("*x");
            if (i > 1) printf("**%d", i);
            if(j % 8 == 0) printf("\n    ");
        }
    } /* for */
    if (j % 8 != 0) printf("\n");

} /* process */

int main(int argc, char **argv)
{
    int opt;
    int n = DEFAULT_GRADE;

    while((opt = getopt(argc, argv, "n:df:")) != EOF) {
        switch(opt) {
        case 'd': flags ^= FLAG_DEBUG; break;
        case 'f': format = optarg; break;
        case 'n': opt = sscanf(optarg, "%d", &n);
            if (opt != 1 || n < 0) {
                fprintf(stderr,
                    "WARNING: n parameter invalid(%s), "
                    "defaulting to %d\n",
                    optarg, DEFAULT_GRADE);
                n = DEFAULT_GRADE;
            } /* if */
            break;
        } /* switch */
    } /* while */

    if (optind < argc) {
        int i;
        for (i = optind; i < argc; i++) {
            FILE *f = fopen(argv[i], "r");
            if (!f) {
                fprintf(stderr,
                        "ERROR: %s: %s(errno=%d)\n",
                        argv[i],
                        strerror(errno),
                        errno);
                exit(EXIT_FAILURE);
            } /* if */
            process(f, n);
            fclose(f);
        } /* for */
    } else {
        process(stdin, n);
    } /* if */

    return EXIT_SUCCESS;
} /* main */

/* $Id: regr_polin.c,v 1.6 2014/04/01 17:22:44 luis Exp $ */
