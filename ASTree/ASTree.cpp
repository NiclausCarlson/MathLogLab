//
// Created by Nik Carlson on 17.07.2020.
//

#include "ASTree.h"

ASTree::ASTree(Expression data, ASTree *left, ASTree *right) : data(std::move(data)), left(left),
                                                                          right(right) {}

void addNode(std::stack<ASTree *> &stack, Expression &op) {
    ASTree *right = stack.top();
    stack.pop();
    ASTree *left = stack.top();
    stack.pop();
    stack.push(new ASTree(op, left, right));
}

void addUnaryNode(std::stack<ASTree *> &stack, Expression &op) {
    ASTree *left = stack.top();
    stack.pop();
    stack.push(new ASTree(op, left, nullptr));
}

void addAloneNode(std::stack<ASTree *> &stack, Expression &op) {
    stack.push(new ASTree(op, nullptr, nullptr));
}

void peekOperations(std::stack<Expression> &operators, std::stack<ASTree *> &result, int priority) {
    if (operators.empty()) return;
    while (!operators.empty() && ((operators.top().getAssoc() == "left" && operators.top().getPriority() >= priority) ||
                                  (operators.top().getAssoc() == "right" &&
                                   operators.top().getPriority() > priority))) {
        Expression exp = operators.top();
        if (exp.getId() == "!" || exp.getId() == "'" || exp.getId()[0] == '@' || exp.getId()[0] == '?') {
            addUnaryNode(result, exp);
        } else {
            addNode(result, exp);
        }
        operators.pop();
    }
}

void peekBracket(std::stack<Expression> &operators, std::stack<ASTree *> &result) {
    while (!operators.empty() && operators.top().getId() != "(") {
        if (operators.top().getId() != "(") {
            if (operators.top().getId() == "!" || operators.top().getId()[0] == '@' ||
                operators.top().getId()[0] == '?' || operators.top().getId() == "'") {
                addUnaryNode(result, operators.top());
            } else {
                addNode(result, operators.top());
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
    if (tree->data.getId() == "&" || tree->data.getId() == "|" ||
        tree->data.getId() == "->" || tree->data.getId() == "+" ||
        tree->data.getId() == "*" || tree->data.getId() == "=") {
        std::string tmp1 = toStringInfix(tree->left);
        std::string tmp2 = toStringInfix(tree->right);
        return "(" + tmp1 + tree->data.getId() + tmp2 + ")";
    } else if (tree->data.getId() == "!") {
        return "(" + tree->data.getId() + toStringInfix(tree->left) + ")";
    } else if (tree->data.getId() == "'") {
        return toStringInfix(tree->left) + tree->data.getId();
    } else if (tree->data.getId()[0] == '@' || tree->data.getId()[0] == '?') {
        return "(" + tree->data.getId() + "." + toStringInfix(tree->left) + ")";
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

void setOccupiedVariables(const ASTree *tree, std::unordered_set<std::string> &variables) {
    if (tree->data.getId()[0] == '@' || tree->data.getId()[0] == '?') {
        variables.insert(tree->data.getId().substr(1));
        if (tree->left != nullptr)
            setOccupiedVariables(tree->left, variables);
        if (tree->right != nullptr)
            setOccupiedVariables(tree->right, variables);
    }
}

void setVariables(const ASTree *tree, std::unordered_set<std::string> &variables) {
    if (tree->data.isVariable()) variables.insert(tree->data.getId());
    else {
        if (tree->left != nullptr) setVariables(tree->left, variables);
        if (tree->right != nullptr) setVariables(tree->right, variables);
    }
}

bool checkFreeOccurs(const ASTree *tree, std::deque<std::string> &quantifiers) {
    if (tree->data.isVariable()) {
        for (const auto &i: quantifiers) {
            if (tree->data.getId() == i) return false;
        }
    }
    if (tree->left != nullptr) return checkFreeOccurs(tree->left, quantifiers);
    if (tree->right != nullptr) return checkFreeOccurs(tree->right, quantifiers);
    return true;
}

bool checkNotFreeOccurs(ASTree *tree, const std::string &variable, std::deque<std::string> &occupiedVariables) {
    if (tree->data.getId()[0] == '@' || tree->data.getId()[0] == '?')
        occupiedVariables.push_back(tree->data.getId().substr(1));

    if (tree->left != nullptr && tree->right != nullptr) {
        bool left = checkNotFreeOccurs(tree->left, variable, occupiedVariables);
        bool right = checkNotFreeOccurs(tree->right, variable, occupiedVariables);

        if (tree->data.getId()[0] == '@' || tree->data.getId()[0] == '?') {
            if (!occupiedVariables.empty())
                occupiedVariables.pop_back();
        }

        return left && right;
    }
    if (tree->left != nullptr) {
        bool left = checkNotFreeOccurs(tree->left, variable, occupiedVariables);
        if (tree->data.getId()[0] == '@' || tree->data.getId()[0] == '?') {
            if (!occupiedVariables.empty())
                occupiedVariables.pop_back();
        }
        return left;
    }

    if (tree->data.isVariable()) {
        if (variable == tree->data.getId()) {
            for (const auto &i:occupiedVariables) {
                if (variable == i) return true;
            }
            return false;
        }
    }
    return true;
}

bool checkNotFreeOccurs(ASTree *tree, const std::string &variable) {
    std::deque<std::string> occupiedVariables;
    return checkNotFreeOccurs(tree, variable, occupiedVariables);
}

bool hardEquals(const ASTree *tree1, const ASTree *tree2) {
    if (tree1->data.getId() == tree2->data.getId()) {
        if ((tree1->left != nullptr && tree2->left != nullptr) &&
            (tree1->right != nullptr && tree2->right != nullptr)) {
            return hardEquals(tree1->left, tree2->left) &&
                   hardEquals(tree1->right, tree2->right);
        }
        if (tree1->left != nullptr && tree2->left != nullptr)
            return hardEquals(tree1->left, tree2->left);

        if (tree1->right != nullptr && tree2->right != nullptr)
            return hardEquals(tree1->right, tree2->right);

        return tree1->left == nullptr && tree2->left == nullptr && tree1->right == nullptr && tree2->right == nullptr;
    }
    return false;
}

bool softEquals(char replacedVariable, const ASTree *tree1, const ASTree *tree2, std::vector<const ASTree *> &terms,
                bool &allFreeForReplace, std::deque<std::string> &quantifiers) {
    if (tree1->data.getId()[1] == replacedVariable) {
        return hardEquals(tree1, tree2);
    }
    if (tree2->data.getId()[0] == '@' || tree2->data.getId()[0] == '?')
        quantifiers.push_back(tree2->data.getId().substr(1));

    if (tree1->data.getId() == tree2->data.getId() && tree1->data.getId()[0] != replacedVariable) {
        if ((tree1->left != nullptr && tree2->left != nullptr) &&
            (tree1->right != nullptr && tree2->right != nullptr)) {
            bool left = softEquals(replacedVariable, tree1->left, tree2->left, terms, allFreeForReplace, quantifiers);
            bool right = softEquals(replacedVariable, tree1->right, tree2->right, terms, allFreeForReplace,
                                    quantifiers);
            if (tree2->data.getId()[0] == '@' || tree2->data.getId()[0] == '?') {
                if (!quantifiers.empty())
                    quantifiers.pop_back();
            }
            return left && right;
        }
        if (tree1->left != nullptr && tree2->left != nullptr) {
            bool left = softEquals(replacedVariable, tree1->left, tree2->left, terms, allFreeForReplace, quantifiers);
            if (tree2->data.getId()[0] == '@' || tree2->data.getId()[0] == '?') {
                if (!quantifiers.empty())
                    quantifiers.pop_back();
            }
            return left;
        }
        return tree1->left == nullptr && tree2->left == nullptr && tree1->right == nullptr && tree2->right == nullptr;
    } else if (tree1->data.isVariable() && tree1->data.getId()[0] == replacedVariable) {
        terms.push_back(tree2);
        allFreeForReplace &= checkFreeOccurs(tree2, quantifiers);
        return true;
    }
    return false;
}

bool isEqualsWithReplace(char replacedVariable, const ASTree *tree1, const ASTree *tree2,
                         std::vector<const ASTree *> &terms, bool &allFreeForReplace) {
    allFreeForReplace = true;
    std::deque<std::string> quantifiers;
    return softEquals(replacedVariable, tree1, tree2, terms, allFreeForReplace, quantifiers);
}


