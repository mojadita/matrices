/* cos.c -- output cosine DFT transformation coefficients in a
 *          way they can be processed by sist_lin program.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Sun May 17 12:56:23 EEST 2020
 * Copyright: (C) 2020 Luis Colorado.  All rights reserved.
 * License: BSD.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define FLAG_ADD_IDENTITY   (1 << 0)

int main(int argc, char **argv)
{
    int n = 8;
    int p = 6;
    int opt;
    double *c;
    int flags = 0;

    while ((opt = getopt(argc, argv, "in:p:")) != EOF) {
        switch (opt) {
        case 'i': flags |= FLAG_ADD_IDENTITY; break;
        case 'n':     n  = atoi(optarg);      break;
        case 'p':     p  = atoi(optarg);      break;
        } /* switch */
    } /* while */

    if (n <  2) n = 2;
    if (p <  3) p = 3;
    if (p > 17) p = 17;

    /* create the matrix */
    c = malloc(n * sizeof *c);

    if (flags & FLAG_ADD_IDENTITY)
        printf("%d %d\n", n, n<<1);
    else
        printf("%d %d\n", n, n);

    int i;
    for (i = 0; i < n; i++) {
        double c2 = 0.0;
        double cmod;
        int j;

        for (j = 0; j < n; j++) {
            double aux = c[j] = cos((0.5 + j)*i*M_PI/(n));
            c2 += aux*aux;
        }
        cmod = sqrt(c2 + (flags & FLAG_ADD_IDENTITY));

        for (j = 0; j < n; j++)
            c[j] /= cmod;

        for (j = 0; j < n; j++) {
            printf("%*.*lf", p+4, p, c[j]);
        } /* for */

        if (flags & FLAG_ADD_IDENTITY)
            for (j = 0; j < n; j++)
                printf(" %*.*lf", p+4, p, (i == j) / cmod);

        printf("\n");

    } /* for */
    printf("\n");

} /* main */
