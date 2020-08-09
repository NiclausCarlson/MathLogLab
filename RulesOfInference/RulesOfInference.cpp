//
// Created by Nik Carlson on 01.08.2020.
//

#include "RulesOfInference.h"

bool isModusPonens(const std::map<std::string, int> &uniqueStatements,
                   const std::map<std::string, std::vector<std::pair<std::string, int>>> &moduses,
                   const std::string &curStatementStr,
                   std::pair<std::string, std::pair<int, int>> &result) {
    if (moduses.empty()) {
        return false;
    }
    if (moduses.count(curStatementStr) != 0) {
        for (const auto &it: moduses.at(curStatementStr)) {
            if (uniqueStatements.count(it.first) != 0) {
                result = std::make_pair(curStatementStr, std::make_pair(it.second, uniqueStatements.at(it.first)));
                return true;
            }
        }
    }
    return false;
}

