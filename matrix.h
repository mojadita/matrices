/* $Id: matrix.h,v 1.2 2014/03/19 14:24:33 luis Exp $
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Mon Mar  3 20:47:47 CET 2014
 * Disclaimer: (C) 2014 LUIS COLORADO SISTEMAS S.L.U.
 *          All rights reserved.
 */

#ifndef _MATRIX_H
#define _MATRIX_H

#ifndef _cplusplus
#error This file only works with C++.
#endif

namespace lcssl {

class tensor<class B> {

    int *dims;
    B v;

public:
    tensor(int[]);
    operator+=(const tensor<B>&);
    operator-=(const tensor<B>&);


}; /* class tensor */

}; /* namespace lcssl */

#endif /* _MATRIX_H */
/* $Id: matrix.h,v 1.2 2014/03/19 14:24:33 luis Exp $ */
