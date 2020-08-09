//
// Created by Nik Carlson on 01.08.2020.
//

#ifndef MATHLOGLAB_RULESOFINFERENCE_H
#define MATHLOGLAB_RULESOFINFERENCE_H

#include "../ASTree/ASTree.h"
#include <vector>
#include <map>
#include <set>

bool isModusPonens(const std::map<std::string, int> &uniqueStatements,
                   const std::map<std::string, std::vector<std::pair<std::string, int>>> &moduses,
                   const std::string &curStatementStr,
                   std::pair<std::string, std::pair<int, int>> &result);

#endif //MATHLOGLAB_RULESOFINFERENCE_H
