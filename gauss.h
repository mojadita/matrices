/* $Id: gauss.h,v 1.1 2014/03/19 14:24:33 luis Exp $
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: miércoles 19 de marzo de 2014 15:01:24 CET
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.
 *	All rights reserved.
 */

#ifndef _GAUSS_H
#define _GAUSS_H

typedef double **matriz;

matriz new_matriz(int nlin, int ncol);

matriz leer_matriz(int nlin, int ncol, char *nombre);

size_t imprime_matriz(matriz m, int nlin, int ncol, double epsilon);

double gauss(matriz A, int nlin, int ncol, double epsilon, int debug);

#endif /* _GAUSS_H */
/* $Id: gauss.h,v 1.1 2014/03/19 14:24:33 luis Exp $ */
