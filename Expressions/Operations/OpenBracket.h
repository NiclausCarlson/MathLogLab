//
// Created by Nik Carlson on 16.07.2020.
//

#ifndef MATHLOGLAB_OPENBRACKET_H
#define MATHLOGLAB_OPENBRACKET_H

#include "../Expression.h"

struct OpenBracket : public Expression {
    OpenBracket() : Expression("(", "not", 0) {}
};

#endif //MATHLOGLAB_OPENBRACKET_H
