//
// Created by Nik Carlson on 22.08.2020.
//

#ifndef MATHLOGLAB_MULTIPLICATION_H
#define MATHLOGLAB_MULTIPLICATION_H

#include "../Expression.h"

struct Multiplication : public Expression {
    Multiplication(): Expression("*", "left", 7, false){}
};

#endif //MATHLOGLAB_MULTIPLICATION_H
