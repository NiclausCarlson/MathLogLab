//
// Created by Nik Carlson on 16.07.2020.
//

#ifndef MATHLOGLAB_EXPRESSION_H
#define MATHLOGLAB_EXPRESSION_H

#include<string>

class Expression {
public:
    Expression();

    Expression(std::string id, std::string assoc, int priority, bool isVariable);

    std::string getId() const;

    std::string getAssoc() const;

    int getPriority() const;

    bool isVariable() const;

private:
    std::string id, assoc;
    int priority{};
    bool isVar{};
};


#endif //MATHLOGLAB_EXPRESSION_H
