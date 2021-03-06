//
// Created by Nik Carlson on 16.07.2020.
//

#include "Expression.h"

Expression::Expression(std::string id, std::string assoc, int priority, bool isVar) : id(std::move(id)),
                                                                                      assoc(std::move(assoc)),
                                                                                      priority(priority),
                                                                                      isVar(isVar) {
}

std::string Expression::getId() const {
    return id;
}

std::string Expression::getAssoc() const {
    return assoc;
}

int Expression::getPriority() const {
    return priority;
}

Expression::Expression() = default;

bool Expression::isVariable() const {
    return isVar;
}
