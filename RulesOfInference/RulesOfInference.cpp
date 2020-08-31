//
// Created by Nik Carlson on 01.08.2020.
//

#include "RulesOfInference.h"

bool isModusPonens(const std::map<std::string, int> &uniqueStatements,
                   const std::map<std::string, std::vector<std::pair<std::string, int>>> &moduses,
                   const std::string &curStatementStr,
                   std::pair<std::string, std::pair<int, int>> &result, bool searchOrder) {
    if (moduses.empty()) return false;

    if (moduses.count(curStatementStr) != 0) {
        if (!searchOrder) {
            for (const auto &it: moduses.at(curStatementStr)) {
                if (uniqueStatements.count(it.first) != 0) {
                    result = std::make_pair(curStatementStr, std::make_pair(it.second, uniqueStatements.at(it.first)));
                    return true;
                }
            }
        } else {
            unsigned long long left1 = 0, right1 = 0, left2 = 0, right2 = 0;
            for (auto it = moduses.at(curStatementStr).rbegin(); it != moduses.at(curStatementStr).rend(); ++it) {
                if (uniqueStatements.count(it->first) != 0) {
                    left1 = uniqueStatements.at(it->first);
                    right1 = it->second;
                    if ((left1 > left2 && right1 > right2) || ((left1 > left2 && right1 <= right2))) {
                        left2 = left1;
                        right2 = right1;
                        result = std::make_pair(curStatementStr,
                                                std::make_pair(left1, right1));
                    }
                }
            }
            return left2 != 0 && right2 != 0;
        }
    }
    return false;
}

int isForAllQuantifierRule(const std::map<std::string, int> &uniqueStatements, ASTree *curStatement,
                           std::pair<std::string, std::pair<int, int>> &result, const std::string &last) {
    if (uniqueStatements.count(last)) {
        std::string var = curStatement->right->data.getId().substr(1);
        if (checkNotFreeOccurs(curStatement->left, var)) {
            result.first = std::string("?@-intro ");
            result.second = std::make_pair(uniqueStatements.at(last), 0);
            return 1;
        } else {
            result.first = std::string("variable ") + var + " occurs free in ?@-rule.";
            result.second = std::make_pair(-1, 0);
            return -1;
        }
    }
    return 0;
}

int isExistQuantifierRule(const std::map<std::string, int> &uniqueStatements, ASTree *curStatement,
                          std::pair<std::string, std::pair<int, int>> &result, const std::string &last) {
    if (uniqueStatements.count(last)) {
        std::string var = curStatement->left->data.getId().substr(1);
        if (checkNotFreeOccurs(curStatement->right, var)) {
            result.first = std::string("?@-intro ");
            result.second = std::make_pair(uniqueStatements.at(last), 0);
            return 1;
        } else {
            result.first = std::string("variable ") + var + " occurs free in ?@-rule.";
            result.second = std::make_pair(-1, 0);
            return -1;
        }
    }
    return 0;
}

bool isQuantifierRule(const std::map<std::string, int> &uniqueStatements, ASTree *curStatement,
                      std::pair<std::string, std::pair<int, int>> &result) {

    if (curStatement->data.getId() == "->") {
        std::string left = toStringInfix(curStatement->left);
        std::string right = toStringInfix(curStatement->right);
        std::pair<std::string, std::pair<int, int>> r1, r2;
        int a = 0, b = 0;
        if (curStatement->left->data.getId()[0] == '?')
            a = isExistQuantifierRule(uniqueStatements, curStatement, r1,
                                      "((" + left.substr(5, left.size() - 6) + "->" + right + ")");
        if (a == 1) {
            result = std::make_pair(r1.first, r1.second);
            return true;
        } else if (a == 0 || a == -1) {
            if (curStatement->right->data.getId()[0] == '@')
                b = isForAllQuantifierRule(uniqueStatements, curStatement, r2,
                                           "(" + left + "->" + right.substr(4));
            if (b == 1 || (a == 0 && b == -1)) {
                result = std::make_pair(r2.first, r2.second);
                return true;
            } else if (a == -1 && (b == 0 || b == -1)) {
                result = std::make_pair(r1.first, r1.second);
                return true;
            }
        }
    }
    return false;
}