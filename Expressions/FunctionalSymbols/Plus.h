//
// Created by Nik Carlson on 22.08.2020.
//

#ifndef MATHLOGLAB_PLUS_H
#define MATHLOGLAB_PLUS_H

#include "../Expression.h"

struct Plus : public Expression {
    Plus(): Expression("+", "left", 6, false){}
};

#endif //MATHLOGLAB_PLUS_H
