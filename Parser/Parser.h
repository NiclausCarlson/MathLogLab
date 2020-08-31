//
// Created by Nik Carlson on 17.07.2020.
//

#ifndef MATHLOGLAB_PARSER_H
#define MATHLOGLAB_PARSER_H

#include <algorithm>

#include "../ASTree/ASTree.h"
#include "../Expressions/Operations/OpenBracket.h"
#include "../Expressions/Operations/Variable.h"
#include "../Expressions/Operations/Conjunction.h"
#include "../Expressions/Operations/Disjunction.h"
#include "../Expressions/Operations/Negation.h"
#include "../Expressions/Operations/Implication.h"
#include "../Expressions/FunctionalSymbols/Hatch.h"
#include "../Expressions/FunctionalSymbols/Multiplication.h"
#include "../Expressions/FunctionalSymbols/Plus.h"
#include "../Expressions/FunctionalSymbols/Zero.h"
#include "../Expressions/PredicateSymbols/Equals.h"
#include "../Expressions/PredicateSymbols/Predicate.h"
#include "../Expressions/Quantifier/Exists.h"
#include "../Expressions/Quantifier/ForEach.h"

Expression getOperator(char ch);

ASTree *parse(std::string &expression, bool mode);

#endif //MATHLOGLAB_PARSER_H
