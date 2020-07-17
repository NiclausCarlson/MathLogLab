//
// Created by Nik Carlson on 16.07.2020.
//

#ifndef MATHLOGLAB_IMPLICATION_H
#define MATHLOGLAB_IMPLICATION_H

#include "../Expression.h"

struct Implication : public Expression {
    Implication() : Expression("->", "right", 1) {}
};

#endif //MATHLOGLAB_IMPLICATION_H
