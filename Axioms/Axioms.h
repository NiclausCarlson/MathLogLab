//
// Created by Nik Carlson on 28.07.2020.
//

#ifndef MATHLOGLAB_AXIOMS_H
#define MATHLOGLAB_AXIOMS_H

#include <vector>
#include "../ASTree/ASTree.h"
#include "../Parser/Parser.h"

bool isLogicAxiomScheme(ASTree *tree, std::pair<std::string, int> &axiom);

bool isPredicateAxiomScheme(ASTree *tree, std::pair<std::string, int> &axiom);

bool isArithmeticAxiom(ASTree *tree, std::pair<std::string, int> &axiom);

bool isArithmeticAxiomScheme(ASTree *tree, std::pair<std::string, int> &axiom);

bool isTermsEquals(std::vector<const ASTree*>& terms);

ASTree* getAxiomTree(int number);

#endif //MATHLOGLAB_AXIOMS_H
