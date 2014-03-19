/* $Id: pru.c,v 1.1 2014/03/19 17:39:19 luis Exp $
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: miércoles 19 de marzo de 2014 14:34:24 CET
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.
 *		All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#define N	100000

double rndom(double n)
{
	double res =  (double)rand()/(double)RAND_MAX;
	return n * (res - 0.5);
}


double f(double x)
{
	static double coefs[] = {
		3.0, 2.0, 1.0,
	};
	static int n_coefs = sizeof coefs/sizeof coefs[0];
	int i;
	double res = 0.0;
	for (i = n_coefs-1; i >= 0; i--) {
		res *= x;
		res += coefs[i];
	} /* for */
	return res /*+ rndom(1.0)*/;
} /* f */

main()
{
	int i;
	for (i = 0; i < N; i++) {
		double x = 100.0*((double)rand() / (double)RAND_MAX);
		printf("%lg\t%lg\n", x, f(x));
	} /* for */
} /* main */

/* $Id: pru.c,v 1.1 2014/03/19 17:39:19 luis Exp $ */
