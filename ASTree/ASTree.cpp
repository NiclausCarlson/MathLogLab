//
// Created by Nik Carlson on 17.07.2020.
//

#include "ASTree.h"

#include <utility>

ASTree::ASTree(Expression data, ASTree *left, ASTree *right) : data(std::move(data)),left(left), right(right) {}

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

std::string toString(const ASTree *tree) {
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

std::string toStringInfix(const ASTree *tree) {
    if (tree->data.getId() == "&") {
        std::string tmp1 = toStringInfix(tree->left);
        std::string tmp2 = toStringInfix(tree->right);
        return "(" + tmp1 + " & " + tmp2 + ")";
    } else if (tree->data.getId() == "|") {
        std::string tmp1 = toStringInfix(tree->left);
        std::string tmp2 = toStringInfix(tree->right);
        return "(" + tmp1 + " | " + tmp2 + ")";
    } else if (tree->data.getId() == "->") {
        std::string tmp1 = toStringInfix(tree->left);
        std::string tmp2 = toStringInfix(tree->right);
        return "(" + tmp1 + " -> " + tmp2 + ")";
    } else if (tree->data.getId() == "!") {
        return "!" + toStringInfix(tree->left);
    } else {
        return tree->data.getId();
    }
}

bool equals(const ASTree *tree1, const ASTree *tree2) {
    if ((tree1 == nullptr) && (tree2 == nullptr)) {
        return true;
    }
    if ((tree1 != nullptr) && (tree2 != nullptr) && tree1->data.getId() == tree2->data.getId()) {
        bool left = equals(tree1->left, tree2->left);
        bool right = equals(tree1->right, tree2->right);
        return left && right;
    }
    return false;
}