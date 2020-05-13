/* $Id: gauss.h,v 1.2 2014/04/01 17:22:44 luis Exp $
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: miércoles 19 de marzo de 2014 15:01:24 CET
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.
 *	All rights reserved.
 */

#ifndef _GAUSS_H
#define _GAUSS_H

#define FLAG_DEBUG			(1 << 0)
#define FLAG_PRINTPIVOTS	(1 << 1)
#define FLAG_DONTDOLASTPASS	(1 << 2)

double	/* returns the determinant value */
gauss(  /* executes Gauss-Jordan on the matrix A */
	double **A, /* matrix to be operated upon. */
	int nlin,   /* rows of matrix. */
	int ncol,   /* columns of matrix. */
	double epsilon, /* value of epsilon under which element is
					 * considered zero. */
	int flags, /* FLAGS, see above */
	const char *fmt); /* format to print each element, "%lg" used
					   * if NULL */

#endif /* _GAUSS_H */
/* $Id: gauss.h,v 1.2 2014/04/01 17:22:44 luis Exp $ */
