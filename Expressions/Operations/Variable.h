//
// Created by Nik Carlson on 16.07.2020.
//

#ifndef MATHLOGLAB_VARIABLE_H
#define MATHLOGLAB_VARIABLE_H

#include "../Expression.h"

struct Variable : public Expression {
    explicit Variable(std::string name) : Expression(std::move(name), "left", 0) {}
};

#endif //MATHLOGLAB_VARIABLE_H
