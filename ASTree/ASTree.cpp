//
// Created by Nik Carlson on 17.07.2020.
//

#include "ASTree.h"

ASTree::ASTree(Expression data, ASTree *left, ASTree *right) : data(std::move(data)), left(left), right(right) {}

void addNode(std::stack<ASTree *> &stack, Expression &op) {
    ASTree *right = stack.top();
    stack.pop();
    ASTree *left = stack.top();
    stack.pop();
    stack.push(new ASTree(op, left, right));
}

void addNegNode(std::stack<ASTree *> &stack, Expression &op) {
    ASTree *left = stack.top();
    stack.pop();
    stack.push(new ASTree(op, left, nullptr));
}

void addAloneNode(std::stack<ASTree *> &stack, Expression &op) {
    stack.push(new ASTree(op, nullptr, nullptr));
}

void peekOperations(std::stack<Expression> &operators, std::stack<ASTree *> &result, int priority) {
    if (operators.empty())return;
    while (!operators.empty() && ((operators.top().getAssoc() == "left" && operators.top().getPriority() >= priority) ||
                                  (operators.top().getAssoc() == "right" &&
                                   operators.top().getPriority() > priority))) {
        Expression exp = operators.top();
        if (exp.getId() == "!") {
            addNegNode(result, exp);
        } else {
            addNode(result, exp);
        }
        operators.pop();
    }
}

void peekBracket(std::stack<Expression> &operators, std::stack<ASTree *> &result) {
    while (!operators.empty() && operators.top().getId() != "(") {
        if (operators.top().getId() != "(") {
            if (operators.top().getId() != "!") {
                addNode(result, operators.top());
            } else {
                addNegNode(result, operators.top());
            }
        }
        operators.pop();
    }
    operators.pop();
}

std::string toString(ASTree *tree) {
    if (tree->data.getId() == "&") {
        std::string tmp1 = toString(tree->left);
        std::string tmp2 = toString(tree->right);
        return "(&," + tmp1 + "," + tmp2 + ")";
    } else if (tree->data.getId() == "|") {
        std::string tmp1 = toString(tree->left);
        std::string tmp2 = toString(tree->right);
        return "(|," + tmp1 + "," + tmp2 + ")";
    } else if (tree->data.getId() == "->") {
        std::string tmp1 = toString(tree->left);
        std::string tmp2 = toString(tree->right);
        return "(->," + tmp1 + "," + tmp2 + ")";
    } else if (tree->data.getId() == "!") {
        return "(!" + toString(tree->left) + ")";
    } else {
        return tree->data.getId();
    }
}