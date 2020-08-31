//
// Created by Nik Carlson on 22.08.2020.
//

#ifndef MATHLOGLAB_PREDICATE_H
#define MATHLOGLAB_PREDICATE_H

#include <utility>

#include "../Expression.h"

struct Predicate : public Expression {
    explicit Predicate(std::string name): Expression(std::move(name), "left", 5, false){}
};
#endif //MATHLOGLAB_PREDICATE_H
