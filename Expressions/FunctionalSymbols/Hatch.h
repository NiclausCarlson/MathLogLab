//
// Created by Nik Carlson on 22.08.2020.
//

#ifndef MATHLOGLAB_HATCH_H
#define MATHLOGLAB_HATCH_H

#include "../Expression.h"

struct Hatch : public Expression {
    Hatch() : Expression("\'", "right", 8, false) {}
};

#endif //MATHLOGLAB_HATCH_H
