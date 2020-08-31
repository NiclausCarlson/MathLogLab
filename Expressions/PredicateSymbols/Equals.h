//
// Created by Nik Carlson on 22.08.2020.
//

#ifndef MATHLOGLAB_EQUALS_H
#define MATHLOGLAB_EQUALS_H

#include "../Expression.h"

struct Equals : public Expression {
    Equals(): Expression("=", "left", 5, false){}
};

#endif //MATHLOGLAB_EQUALS_H
