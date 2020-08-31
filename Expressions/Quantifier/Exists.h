//
// Created by Nik Carlson on 22.08.2020.
//

#ifndef MATHLOGLAB_EXISTS_H
#define MATHLOGLAB_EXISTS_H

#include "../Expression.h"

struct Exists : public Expression {
    explicit Exists(std::string name) : Expression(std::move(name), "none", 9, false) {}
};

#endif //MATHLOGLAB_EXISTS_H
