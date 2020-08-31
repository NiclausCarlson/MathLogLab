//
// Created by Nik Carlson on 22.08.2020.
//

#ifndef MATHLOGLAB_ZERO_H
#define MATHLOGLAB_ZERO_H

#include "../Expression.h"

struct Zero : public Expression {
    Zero() : Expression("0", "none", 9, false) {}
};

#endif //MATHLOGLAB_ZERO_H
