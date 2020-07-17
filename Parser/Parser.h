//
// Created by Nik Carlson on 17.07.2020.
//

#ifndef MATHLOGLAB_PARSER_H
#define MATHLOGLAB_PARSER_H


#include "../ASTree/ASTree.h"
#include "../Expressions/Operations/OpenBracket.h"
#include "../Expressions/Operations/Variable.h"
#include "../Expressions/Operations/Conjunction.h"
#include "../Expressions/Operations/Disjunction.h"
#include "../Expressions/Operations/Negation.h"
#include "../Expressions/Operations/Implication.h"

Expression getOperator(char ch);
ASTree *parse(std::string &expression);

#endif //MATHLOGLAB_PARSER_H
