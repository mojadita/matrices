/* $Id: io.h,v 1.1 2014/04/01 17:22:44 luis Exp $
 * vim: ts=4 sw=4 nowrap
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Tue Apr  1 19:16:12 CEST 2014
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.  All
 * rights reserved.
 */

#ifndef _IO_H
#define _IO_H

typedef double **matriz;

int
leer_entero(
	char *prompt,
	int min,
	int max);

matriz
new_matriz(
	int nlin,
	int ncol);

matriz
leer_matriz(
	int nlin,
	int ncol,
	char *nombre);

int
imprime_matriz(
		matriz m,
		int filas,
		int columnas,
		const char *fmt,
		double eps);

#endif /* _IO_H */
/* $Id: io.h,v 1.1 2014/04/01 17:22:44 luis Exp $ */
