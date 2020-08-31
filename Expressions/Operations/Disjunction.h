//
// Created by Nik Carlson on 16.07.2020.
//
#ifndef MATHLOGLAB_DISJUNCTION_H
#define MATHLOGLAB_DISJUNCTION_H

#include "../Expression.h"

struct Disjunction : public Expression {
    Disjunction() : Expression("|", "left", 2, false) {}
};

#endif //MATHLOGLAB_DISJUNCTION_H
