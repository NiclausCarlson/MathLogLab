//
// Created by Nik Carlson on 28.07.2020.
//

#include "Axioms.h"

bool isTermsEquals(std::vector<const ASTree *> &terms) {
    for (auto begin = terms.begin() + 1; begin != terms.end(); ++begin) {
        if (!equals(terms[0], *begin)) {
            return false;
        }
    }
    return true;
}

// Logic Axiom Schemes
// Ax. sch. 1:  A->B->A
// Ax. sch. 2:  (A->B)->(A->B->C)->(A->C)
// Ax. sch. 3 || Ax. sch. 5:  A->B->A&B
// Ax. sch. 4 || Ax. sch. 3:  A&B->A
// Ax. sch. 5 || Ax. sch. 4:  A&B->B
// Ax. sch. 6:  A->A|B
// Ax. sch. 7:  B->A|B
// Ax. sch. 8:  (A->C)->(B->C)->(A|B->C)
// Ax. sch. 9:  (A->B)->(A->!B)->!A
// Ax. sch. 10: !(!A)->A

bool isLogicAxiomScheme(ASTree *tree, std::pair<std::string, int> &axiom) {
    if (tree->data.getId() == "->" && tree->right->data.getId() == "->") {
        if (equals(tree->left, tree->right->right)) {
            axiom = std::make_pair("Ax. sch. ", 1);
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
            axiom = std::make_pair("Ax. sch. ", 2);
            return true;
        }
    }

    if (tree->data.getId() == "->" &&
        tree->right->data.getId() == "->" &&
        tree->right->right->data.getId() == "&") {
        if (equals(tree->left, tree->right->right->left) &&
            equals(tree->right->left, tree->right->right->right)) {
            axiom = std::make_pair("Ax. sch. ", 5);
            return true;
        }
    }

    if (tree->data.getId() == "->" && tree->left->data.getId() == "&") {
        if (equals(tree->left->left, tree->right)) {
            axiom = std::make_pair("Ax. sch. ", 3);
            return true;
        } else if (equals(tree->left->right, tree->right)) {
            axiom = std::make_pair("Ax. sch. ", 4);
            return true;
        }
    }

    if (tree->data.getId() == "->" && tree->right->data.getId() == "|") {
        if (equals(tree->left, tree->right->left)) {
            axiom = std::make_pair("Ax. sch. ", 6);
            return true;
        } else if (equals(tree->left, tree->right->right)) {
            axiom = std::make_pair("Ax. sch. ", 7);
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
            axiom = std::make_pair("Ax. sch. ", 8);
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
            axiom = std::make_pair("Ax. sch. ", 9);
            return true;
        }
    }

    if (tree->data.getId() == "->" &&
        tree->left->data.getId() == "!" && tree->left->left->data.getId() == "!") {
        if (equals(tree->left->left->left, tree->right)) {
            axiom = std::make_pair("Ax. sch. ", 10);
            return true;
        }
    }
    return false;
}

// Predicate Axiom Schemes
// Ax. sch. 11: (@x.ψ)->ψ[x := Θ]
// Ax. sch. 12: ψ[x := Θ]->?x.ψ

bool isPredicateAxiomScheme(ASTree *tree, std::pair<std::string, int> &axiom) {
    if (tree->data.getId() == "->") {
        std::vector<const ASTree *> terms;
        std::string var;
        bool isAllFreeForReplace;
        if (tree->left->data.getId()[0] == '@' &&
            isEqualsWithReplace(tree->left->data.getId()[1], tree->left->left, tree->right, terms,
                                isAllFreeForReplace)) {
            if (terms.empty()) {
                axiom = std::make_pair("Ax. sch. ", 11);
                return true;
            }
            if (isTermsEquals(terms)) {
                if (isAllFreeForReplace) {
                    axiom = std::make_pair("Ax. sch. ", 11);
                } else {
                    axiom = std::make_pair(
                            axiom.first = "variable " + tree->left->data.getId().substr(1) + " is not free for term " +
                                          toStringInfix(terms[0]) + " in ?@-axiom.",
                            axiom.second = -1);
                }
                return true;
            }
        }
        if (tree->right->data.getId()[0] == '?' &&
            isEqualsWithReplace(tree->right->data.getId()[1], tree->right->left, tree->left, terms,
                                isAllFreeForReplace)) {
            if (terms.empty()) {
                axiom = std::make_pair("Ax. sch. ", axiom.second = 12);
                return true;
            }
            if (isTermsEquals(terms)) {
                if (isAllFreeForReplace) {
                    axiom = std::make_pair("Ax. sch. ", axiom.second = 12);
                } else {
                    axiom = std::make_pair(
                            "variable " + tree->right->data.getId().substr(1) + " is not free for term " +
                            toStringInfix(terms[0]) + " in ?@-axiom.", -1);
                }
                return true;
            }
        }
    }
    return false;
}

// Arithmetic Axioms
// Ax.1:    (a=b)->((a=c)->(b=c))
// Ax.2:    (a=b)->(a'=b')
// Ax.3:    (a'=b')->(a=b)
// Ax.4:    !(a'=0)
// Ax.5:    (a+0)=a
// Ax.6:    (a+b')=(a+b)'
// Ax.7:    (a*0)=0
// Ax.8:    (a*b')=(a*b+a)

bool isArithmeticAxiom(ASTree *tree, std::pair<std::string, int> &axiom) {
    if (tree->data.getId() == "->" &&
        tree->left->data.getId() == "=" && tree->right->data.getId() == "->" &&
        tree->right->left->data.getId() == "=" && tree->right->right->data.getId() == "=") {
        if (tree->left->left->data.isVariable() && tree->left->right->data.isVariable() &&
            tree->right->right->right->data.isVariable()) {
            if (tree->left->left->data.getId()[0] == 'a' && tree->left->right->data.getId()[0] == 'b' &&
                tree->right->left->right->data.getId()[0] == 'c' &&
                tree->left->left->data.getId() == tree->right->left->left->data.getId() &&
                tree->left->right->data.getId() == tree->right->right->left->data.getId() &&
                tree->right->left->right->data.getId() == tree->right->right->right->data.getId()) {
                axiom = std::make_pair("Ax. A", 1);
                return true;
            }
        }
    }
    if (tree->data.getId() == "->" &&
        tree->left->data.getId() == "=" && tree->right->data.getId() == "=" &&
        tree->right->left->data.getId() == "'" && tree->right->right->data.getId() == "'") {
        if (tree->left->left->data.isVariable() && tree->left->right->data.isVariable()) {
            if (tree->left->left->data.getId()[0] == 'a' && tree->left->right->data.getId()[0] == 'b' &&
                tree->left->left->data.getId() == tree->right->left->left->data.getId() &&
                tree->left->right->data.getId() == tree->right->right->left->data.getId()) {
                axiom = std::make_pair("Ax. A", 2);
                return true;
            }
        }
    }
    if (tree->data.getId() == "->" &&
        tree->left->data.getId() == "=" && tree->right->data.getId() == "=" &&
        tree->left->left->data.getId() == "'" && tree->left->right->data.getId() == "'") {
        if (tree->left->left->left->data.isVariable() && tree->left->right->left->data.isVariable()) {
            if (tree->right->left->data.getId()[0] == 'a' && tree->right->right->data.getId()[0] == 'b' &&
                tree->left->left->left->data.getId() == tree->right->left->data.getId() &&
                tree->left->right->left->data.getId() == tree->right->right->data.getId()) {
                axiom = std::make_pair("Ax. A", 3);
                return true;
            }
        }
    }
    if (tree->data.getId() == "!" && tree->left->data.getId() == "=" &&
        tree->left->right->data.getId()[0] == '0' && tree->left->left->data.getId() == "'" &&
        tree->left->left->left->data.isVariable() && tree->left->left->left->data.getId()[0] == 'a') {
        axiom = std::make_pair("Ax. A", 4);
        return true;
    }
    if (tree->data.getId() == "=" &&
        tree->left->data.getId() == "+" && tree->right->data.isVariable() &&
        tree->left->left->data.isVariable() && tree->left->right->data.getId()[0] == '0' &&
        tree->right->data.getId()[0] == 'a' &&
        tree->right->data.getId() == tree->left->left->data.getId()) {
        axiom = std::make_pair("Ax. A", 5);
        return true;
    }
    if (tree->data.getId() == "=" &&
        tree->left->data.getId() == "+" && tree->right->data.getId() == "'" && tree->right->left->data.getId() == "+" &&
        tree->left->left->data.isVariable() && tree->left->right->left->data.isVariable()) {
        if (tree->left->left->data.getId()[0] == 'a' && tree->left->right->left->data.getId()[0] == 'b' &&
            tree->left->left->data.getId() == tree->right->left->left->data.getId() &&
            tree->left->right->left->data.getId() == tree->right->left->right->data.getId()) {
            axiom = std::make_pair("Ax. A", 6);
            return true;
        }
    }
    if (tree->data.getId() == "=" &&
        tree->left->data.getId() == "*" && tree->right->data.getId()[0] == '0' &&
        tree->left->left->data.isVariable() && tree->left->left->data.getId()[0] == 'a' &&
        tree->left->right->data.getId()[0] == '0') {
        axiom = std::make_pair("Ax. A", 7);
        return true;
    }
    if (tree->data.getId() == "=" &&
        tree->left->data.getId() == "*" && tree->right->data.getId() == "+" &&
        tree->left->right->data.getId() == "'" && tree->right->left->data.getId() == "*" &&
        tree->left->left->data.isVariable() && tree->left->right->left->data.isVariable()) {
        if (tree->left->left->data.getId()[0] == 'a' && tree->left->right->left->data.getId()[0] == 'b' &&
            tree->left->left->data.getId() == tree->right->left->left->data.getId() &&
            tree->left->left->data.getId() == tree->right->right->data.getId() &&
            tree->left->right->left->data.getId() == tree->right->left->right->data.getId()) {
            axiom = std::make_pair("Ax. A", 8);
            return true;
        }
    }
    return false;
}

// Arithmetic Axiom Scheme(induction)
// (ψ[x := 0]&(@x.ψ -> ψ[x := x'])) -> ψ

bool isArithmeticAxiomScheme(ASTree *tree, std::pair<std::string, int> &axiom) {
    if (tree->data.getId() == "->" && tree->left->data.getId() == "&" &&
        tree->left->right->data.getId()[0] == '@' && tree->left->right->left->data.getId() == "->") {
        std::vector<const ASTree *> psi;
        psi.push_back(tree->right);
        psi.push_back(tree->left->left);
        psi.push_back(tree->left->right->left->left);
        psi.push_back(tree->left->right->left->right);
        char var = tree->left->right->data.getId()[1];
        std::vector<const ASTree *> terms;
        bool tmp = true;
        for (auto i = 1; i < 4; ++i) {
            if (!isEqualsWithReplace(var, psi[0], psi[i], terms, tmp)) return false;
            if (!terms.empty() && !isTermsEquals(terms)) return false;
            if (i == 1 && !terms.empty() && terms[0]->data.getId() != "0") return false;
            if (i != 3)
                terms.clear();
        }
        for (auto i:terms) {
            if (!(i->data.getId() == "'" && i->left->data.isVariable())) {
                return false;
            }
        }
        axiom = std::make_pair("Ax. sch. A", 9);
        return true;
    }
    return false;
}

ASTree *getAxiomTree(int number) {
    std::string str;
    if (number == 1) {
        str = "P->(Q->P)";
    } else if (number == 2) {
        str = "(P->Q)->(P->Q->C)->(P->C)";
    } else if (number == 8) {
        str = "(P->Q)->(C->Q)->(P|C->Q)";
    }
    return parse(str);
}
