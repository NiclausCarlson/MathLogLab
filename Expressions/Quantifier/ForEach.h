//
// Created by Nik Carlson on 22.08.2020.
//

#ifndef MATHLOGLAB_FOREACH_H
#define MATHLOGLAB_FOREACH_H

#include <utility>

#include "../Expression.h"

struct ForEach : public Expression {
    explicit ForEach(std::string name) : Expression(std::move(name), "none", 9, false) {}
};

#endif //MATHLOGLAB_FOREACH_H
