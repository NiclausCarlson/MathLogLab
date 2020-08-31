//
// Created by Nik Carlson on 16.07.2020.
//
#ifndef MATHLOGLAB_NEGATION_H
#define MATHLOGLAB_NEGATION_H

#include "../Expression.h"

struct Negation : public Expression {
    Negation() : Expression("!", "right", 4, false) {}
};

#endif //MATHLOGLAB_NEGATION_H
