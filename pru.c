/* $Id: pru.c,v 1.2 2014/03/19 21:57:23 luis Exp $
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Wed mar 19 2014 14:34:24 CET
 * Copyright: (C) 2014-2020 LUIS COLORADO SISTEMAS S.L.U.
 *      All rights reserved.
 * License: BSD.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N   999

double rndom()
{
    double res =  (double)rand()/(double)RAND_MAX;
    return (res - 0.5);
}


double f(double x)
{
    double res = 0.0;
#if 1
    static double coefs[] = {
        -1.0, 3.0, 2.0, 1.0, 1.0,
    };
    static int n_coefs = sizeof coefs/sizeof coefs[0];
    int i;
    for (i = n_coefs-1; i >= 0; i--) {
        res *= x;
        res += coefs[i];
    } /* for */
#endif
    return res /*+ rndom()*/;
} /* f */

main()
{
    int i;
    for (i = 0; i < N; i++) {
        double x = 10.0*((double)rand() / (double)RAND_MAX);
        printf("%lg\t%lg\n", x, f(x));
    } /* for */
} /* main */

/* $Id: pru.c,v 1.2 2014/03/19 21:57:23 luis Exp $ */
