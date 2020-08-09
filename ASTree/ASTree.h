//
// Created by Nik Carlson on 17.07.2020.
//

#ifndef MATHLOGLAB_ASTREE_H
#define MATHLOGLAB_ASTREE_H


#include <stack>
#include "../Expressions/Expression.h"

struct ASTree {
    ASTree(Expression data, ASTree *left, ASTree *right);

    Expression data;
    ASTree *left, *right;
};

void addNode(std::stack<ASTree *> &stack, Expression &op);

void addNegNode(std::stack<ASTree *> &stack, Expression &op);

void addAloneNode(std::stack<ASTree *> &stack, Expression &op);

void peekOperations(std::stack<Expression> &operators, std::stack<ASTree *> &result, int priority);

void peekBracket(std::stack<Expression> &operators, std::stack<ASTree *> &result);

std::string toString(const ASTree *tree);

std::string toStringInfix(const ASTree *tree);

bool equals(const ASTree *tree1, const ASTree *tree2);

#endif //MATHLOGLAB_ASTREE_H
