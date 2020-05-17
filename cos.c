#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define N   1024

#ifndef TEST
#define TEST 0
#endif

int main(int argc, char **argv)
{
    int i;
    double c[N][N];
    int n = 8;
    int opt;
    int p = 6;

    while ((opt = getopt(argc, argv, "n:p:")) != EOF) {
        switch (opt) {
        case 'n': n = atoi(optarg); break;
        case 'p': p = atoi(optarg); break;
        } /* switch */
    } /* while */

    if (n >  N) n = N;
    if (n <  2) n = 2;

    if (p <  3) p = 3;
    if (p > 17) p = 17;

    printf("%d %d\n", n, n<<1);
    for (i = 0; i < n; i++) {
        double c2 = 0.0;
        double cmod;
        int j;

        for (j = 0; j < n; j++) {
            double aux = c[i][j] = cos((0.5 + j)*i*M_PI/(n));
            c2 += aux*aux;
        }
        cmod = sqrt(c2);

        for (j = 0; j < n; j++)
            c[i][j] /= cmod;

        for (j = 0; j < n; j++) {
            printf("%*.*lf", p+4, p, c[i][j]);
        } /* for */

        for (j = 0; j < n; j++)
            printf(" %d", (i == j));

        printf("\n");

    } /* for */
    printf("\n");

#if TEST
    for (i = 0; i < n; i++) {
        int j;
        for (j = 0; j < n; j++) {
            int k;
            double a_ij = 0.0;
            for (k = 0; k < n; k++)
                a_ij += c[i][k]*c[j][k];
            printf("%6.2lf",
                    fabs(a_ij) > 1.0e-10 ? a_ij : 0.0);
        } /* for */
        printf("\n");
    } /* for */
#endif
} /* main */
