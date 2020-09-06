//
// Created by Nik Carlson on 01.09.2020.
//

#ifndef MATHLOGLAB_PRIMITIVEPROOFS_H
#define MATHLOGLAB_PRIMITIVEPROOFS_H


#include <string>
#include <vector>
#include "../ASTree/ASTree.h"
#include "../Parser/Parser.h"

class PrimitiveProofs {

public:

    PrimitiveProofs();

    std::vector<ASTree *> getWithReplace(int index, ASTree *p, ASTree *q);

    std::vector<ASTree *> getWithReplace(int index, ASTree *p);

    void insertWithReplace(int index, ASTree *p, std::vector<ASTree *> &result);

private:
    std::vector<std::vector<std::string>> primitives;
    std::vector<std::vector<ASTree *>> parsedPrimitives;
};


#endif //MATHLOGLAB_PRIMITIVEPROOFS_H
