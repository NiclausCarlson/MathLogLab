//
// Created by Nik Carlson on 17.07.2020.
//

#ifndef MATHLOGLAB_ASTREE_H
#define MATHLOGLAB_ASTREE_H


#include <stack>
#include <vector>
#include <algorithm>
#include "unordered_set"
#include "../Expressions/Expression.h"

struct ASTree {
    ASTree(Expression data, ASTree *left, ASTree *right);

    Expression data;
    ASTree *left, *right;
};

void addNode(std::stack<ASTree *> &stack, Expression &op);

void addUnaryNode(std::stack<ASTree *> &stack, Expression &op);

void addAloneNode(std::stack<ASTree *> &stack, Expression &op);

void peekOperations(std::stack<Expression> &operators, std::stack<ASTree *> &result, int priority);

void peekBracket(std::stack<Expression> &operators, std::stack<ASTree *> &result);

std::string toString(const ASTree *tree);

std::string toStringInfix(const ASTree *tree);

bool equals(const ASTree *tree1, const ASTree *tree2);

bool checkFreeOccurs(const ASTree *tree, std::deque<std::string> &occupiedVariables);

bool checkNotFreeOccurs(ASTree *tree, const std::string &variable, std::deque<std::string> &occupiedVariables);

bool checkNotFreeOccurs(ASTree *tree, const std::string &variable);

void setOccupiedVariables(const ASTree *tree, std::unordered_set<std::string> &variables);

void setVariables(const ASTree *tree, std::unordered_set<std::string> &variables);

bool hardEquals(const ASTree *tree1, const ASTree *tree2);

bool softEquals(char replacedVariable, const ASTree *tree1, const ASTree *tree2, std::vector<const ASTree *> &terms,
                bool &allFreeForReplace, std::deque<std::string> &quantifiers);

bool isEqualsWithReplace(char replacedVariable, const ASTree *tree1, const ASTree *tree2,
                         std::vector<const ASTree *> &terms, bool &allFreeForReplace);

bool eval(const ASTree *tree, bool a, bool b, bool c, const std::vector<std::string> &variables);

void replace(ASTree *&tree, ASTree *replaced1, ASTree *replaced2,ASTree *replaced3);

void replace(ASTree *&tree, ASTree *replaced1, ASTree *replaced2);

void replace(ASTree *&tree, ASTree *replaced);

ASTree *getCopy(ASTree *tree);

#endif //MATHLOGLAB_ASTREE_H
