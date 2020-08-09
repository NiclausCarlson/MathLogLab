//
// Created by Nik Carlson on 28.07.2020.
//

#ifndef MATHLOGLAB_AXIOMS_H
#define MATHLOGLAB_AXIOMS_H

#include <vector>
#include "../ASTree/ASTree.h"
#include "../Parser/Parser.h"

bool isAxiom(ASTree *tree, std::pair<std::string, int> &axiom);

std::vector<ASTree *> parseAxiom(ASTree *tree, const int axiomNumber);

#endif //MATHLOGLAB_AXIOMS_H
