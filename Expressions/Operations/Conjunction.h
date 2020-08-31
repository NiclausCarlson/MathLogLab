//
// Created by Nik Carlson on 16.07.2020.
//

#ifndef MATHLOGLAB_CONJUNCTION_H
#define MATHLOGLAB_CONJUNCTION_H

#include "../Expression.h"

struct Conjunction : public Expression {
    Conjunction() : Expression("&", "left", 3, false) {}
};


#endif //MATHLOGLAB_CONJUNCTION_H
