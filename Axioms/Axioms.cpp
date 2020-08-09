//
// Created by Nik Carlson on 28.07.2020.
//

#include "Axioms.h"


// Ax. sch. 1:   A->B->A
// Ax. sch. 2:   (A->B)->(A->B->C)->(A->C)
// Ax. sch. 3:   A->B->A&B
// Ax. sch. 4:   A&B->A
// Ax. sch. 5:   A&B->B
// Ax. sch. 6:   A->A|B
// Ax. sch. 7:   B->A|B
// Ax. sch. 8:   (A->C)->(B->C)->(A|B->C)
// Ax. sch. 9:   (A->B)->(A->!B)->!A
// Ax. sch. 10:  !(!A)->A


bool isAxiom(ASTree *tree, std::pair<std::string, int> &axiom) {

    if (tree->data.getId() == "->" && tree->right->data.getId() == "->") {
        if (equals(tree->left, tree->right->right)) {
            axiom = std::make_pair("Ax. sch.", 1);
            return true;
        }
    }

    if (tree->data.getId() == "->" &&
        tree->left->data.getId() == "->" && tree->right->data.getId() == "->" &&
        tree->right->left->data.getId() == "->" && tree->right->right->data.getId() == "->" &&
        tree->right->left->right->data.getId() == "->") {

        if (equals(tree->left->left, tree->right->left->left) &&
            equals(tree->left->left, tree->right->right->left) &&
            equals(tree->left->right, tree->right->left->right->left) &&
            equals(tree->right->left->right->right, tree->right->right->right)) {
            axiom = std::make_pair("Ax. sch.", 2);
            return true;
        }
    }

    if (tree->data.getId() == "->" &&
        tree->right->data.getId() == "->" &&
        tree->right->right->data.getId() == "&") {
        if (equals(tree->left, tree->right->right->left) &&
            equals(tree->right->left, tree->right->right->right)) {
            axiom = std::make_pair("Ax. sch.", 3);
            return true;
        }
    }

    if (tree->data.getId() == "->" && tree->left->data.getId() == "&") {
        if (equals(tree->left->left, tree->right)) {
            axiom = std::make_pair("Ax. sch.", 4);
            return true;
        } else if (equals(tree->left->right, tree->right)) {
            axiom = std::make_pair("Ax. sch.", 5);
            return true;
        }
    }

    if (tree->data.getId() == "->" && tree->right->data.getId() == "|") {
        if (equals(tree->left, tree->right->left)) {
            axiom = std::make_pair("Ax. sch.", 6);
            return true;
        } else if (equals(tree->left, tree->right->right)) {
            axiom = std::make_pair("Ax. sch.", 7);
            return true;
        }
    }

    if (tree->data.getId() == "->" &&
        tree->left->data.getId() == "->" && tree->right->data.getId() == "->" &&
        tree->right->left->data.getId() == "->" && tree->right->right->data.getId() == "->" &&
        tree->right->right->left->data.getId() == "|") {
        if (equals(tree->left->left, tree->right->right->left->left) &&
            equals(tree->left->right, tree->right->left->right) &&
            equals(tree->left->right, tree->right->right->right) &&
            equals(tree->right->left->left, tree->right->right->left->right)) {
            axiom = std::make_pair("Ax. sch.", 8);
            return true;
        }
    }

    if (tree->data.getId() == "->" &&
        tree->left->data.getId() == "->" && tree->right->data.getId() == "->" &&
        tree->right->left->data.getId() == "->" && tree->right->right->data.getId() == "!" &&
        tree->right->left->right->data.getId() == "!") {
        equals(tree->left->left, tree->right->left->left);
        equals(tree->left->left, tree->right->right->left);
        equals(tree->left->right, tree->right->left->right->left);
        if (equals(tree->left->left, tree->right->left->left) &&
            equals(tree->left->left, tree->right->right->left) &&
            equals(tree->left->right, tree->right->left->right->left)) {
            axiom = std::make_pair("Ax. sch.", 9);
            return true;
        }
    }

    if (tree->data.getId() == "->" &&
        tree->left->data.getId() == "!" && tree->left->left->data.getId() == "!") {
        if (equals(tree->left->left->left, tree->right)) {
            axiom = std::make_pair("Ax. sch.", 10);
            return true;
        }
    }
    return false;
}

std::vector<ASTree *> parseAxiom(ASTree *tree, const int axiomNumber) {
    std::vector<ASTree *> result;
    if (axiomNumber == 1 || axiomNumber == 3) {
        result.push_back(tree->left);
        result.push_back(tree->right->left);
    } else if (axiomNumber == 2) {
        result.push_back(tree->left->left);
        result.push_back(tree->left->right);
        result.push_back(tree->right->right->right);
    } else if (axiomNumber == 4 || axiomNumber == 5 || axiomNumber == 9) {
        result.push_back(tree->left->left);
        result.push_back(tree->left->right);
    } else if (axiomNumber == 6 || axiomNumber == 7) {
        result.push_back(tree->right->left);
        result.push_back(tree->right->right);
    } else if (axiomNumber == 8) {
        result.push_back(tree->left->left);
        result.push_back(tree->left->right);
        result.push_back(tree->right->left->left);
    } else if (axiomNumber == 10) {
        result.push_back(tree->right);
    }
    return result;
}