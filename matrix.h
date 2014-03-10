/* $Id: matrix.h,v 1.1.1.1 2014/03/10 09:22:27 luis Exp $
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Mon Mar  3 20:47:47 CET 2014
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.
 * 			All rights reserved.
 */

typedef struct matriz {
	int l, c;
	double **v;
} *matriz;

matriz new_matriz(int l, int c);

void free_matriz(matriz m);

/* $Id: matrix.h,v 1.1.1.1 2014/03/10 09:22:27 luis Exp $ */
