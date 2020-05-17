/* $Id: io.h,v 1.1 2014/04/01 17:22:44 luis Exp $
 * vim: ts=4 sw=4 nowrap
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Tue Apr  1 19:16:12 CEST 2014
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.  All
 * rights reserved.
 */

#ifndef _IO_H
#define _IO_H

int
read_int(
    char *      prompt,
    int         min,
    int         max);

double **
new_matrix(
    int         rows,
    int         cols);

double **
read_matrix(
    int         rows,
    int         cols,
    char *      name);

int
print_matrix(
        double **   mat,
        int         rows,
        int         cols,
        const char *fmt,
        double      epsilon);

#endif /* _IO_H */
/* $Id: io.h,v 1.1 2014/04/01 17:22:44 luis Exp $ */
